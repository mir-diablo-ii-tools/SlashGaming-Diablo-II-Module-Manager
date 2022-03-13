/**
 * SlashGaming Diablo II Module Manager
 * Copyright (C) 2020-2022  Mir Drualga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "module.h"

#include <stddef.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include "hash/hash_sha1.h"
#include "hash/hash_sha384.h"
#include "win9x.h"

#define MODULE_EXTENSION L".dll"

enum {
  Module_kExtensionLength = sizeof(MODULE_EXTENSION)
      / sizeof(MODULE_EXTENSION[0])
      - 1,
  Module_kSignatureExtensionLength = HashSha384_kSignatureFileExtensionLength,
};

static int IsValidFile(
    const wchar_t* path,
    const wchar_t* expected_extension) {
  size_t path_length;
  const wchar_t* extension;

  if (!PathFileExistsW(path)) {
    return 0;
  }

  if (PathIsDirectoryW(path)) {
    return 0;
  }

  path_length = wcslen(path);
  if (path_length > MAX_PATH - 1) {
    return 0;
  }

  extension = PathFindExtensionW(path);
  if (_wcsicmp(extension, expected_extension) != 0) {
    return 0;
  }

  return 1;
}

/**
 * External
 */

struct Module Module_Init(const wchar_t* path) {
  struct Module module;

  size_t path_length;
  size_t writable_characters;

  module.path[0] = L'\0';
  wcsncat(module.path, path, MAX_PATH - 1);

  module.handle = NULL;

  return module;
}

void Module_Deinit(struct Module* module) {
}

void Module_Load(struct Module* module) {
  module->handle = LoadLibraryW(module->path);
}

int Module_LoadVerified(struct Module* module, const wchar_t* signatures_dir) {
  int is_locate_signature_success;
  int is_verified;

  wchar_t signature_path[MAX_PATH];

  is_locate_signature_success = Module_LocateSignature(
      module,
      signature_path,
      signatures_dir);
  if (!is_locate_signature_success) {
    return 0;
  }

  is_verified = Module_VerifySignature(module, signature_path);
  if (is_verified) {
    Module_Load(module);
  }

  return is_verified;
}

int Module_LocateSignature(
    const struct Module* module,
    wchar_t* signature_path,
    const wchar_t* signatures_dir) {
  wchar_t* path_combine_result;
  BOOL is_path_rename_extension_success;

  const wchar_t* module_file_name;
  size_t signatures_dir_length;
  size_t module_file_name_length;
  size_t signature_path_length;
  const wchar_t* signature_extension;

  module_file_name = PathFindFileNameW(module->path);

  /* Check that the signature path length does not exceed MAX_PATH. */
  signatures_dir_length = wcslen(signatures_dir);
  module_file_name_length = wcslen(module_file_name);
  signature_path_length = signatures_dir_length
      + (module_file_name_length - Module_kExtensionLength)
      + Module_kSignatureExtensionLength;
  if (signature_path_length >= MAX_PATH) {
    return 0;
  }

  /* Create the signature path string. */
  path_combine_result = PathCombineW(
      signature_path,
      signatures_dir,
      module_file_name);
  if (path_combine_result == NULL) {
    return 0;
  }

  if (Win9x_IsRunningOs()) {
    signature_extension = HASH_SHA1_SIGNATURE_FILE_EXTENSION;
  } else {
    signature_extension = HASH_SHA384_SIGNATURE_FILE_EXTENSION;
  }

  is_path_rename_extension_success = PathRenameExtensionW(
      signature_path,
      signature_extension);
  if (!is_path_rename_extension_success) {
    return 0;
  }

  return 1;
}

int Module_VerifySignature(
    const struct Module* module,
    const wchar_t* signature_path) {
  int is_locate_signature_success;

  if (Win9x_IsRunningOs()) {
    return HashSha1_VerifySignatureFile(module->path, signature_path);
  } else {
    return HashSha384_VerifySignatureFile(module->path, signature_path);
  }
}

int Module_IsValid(const wchar_t* path) {
  return IsValidFile(path, MODULE_EXTENSION);
}

int Module_IsSignatureValid(const wchar_t* path) {
  const wchar_t* module_signature_extension;
  if (Win9x_IsRunningOs()) {
    module_signature_extension = HASH_SHA1_SIGNATURE_FILE_EXTENSION;
  } else {
    module_signature_extension = HASH_SHA384_SIGNATURE_FILE_EXTENSION;
  }

  return IsValidFile(path, module_signature_extension);
}
