/**
 * SlashGaming Diablo II Module Manager
 * Copyright (C) 2020-2022  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Module Manager.
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

#include "module_manager_struct.h"

#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include "../config.h"
#include "../error.h"
#include "../filew.h"
#include "../file/file_count.h"
#include "../file/file_list.h"
#include "../module.h"

#define MODULE_DIR_NAME L"Module"
#define MODULE_SIGNATURE_DIR_NAME L"Signature"
#define MODULE_CONFIG_DIR_NAME L"Config"

enum {
  kModuleDirNameLength =
      sizeof(MODULE_DIR_NAME) / sizeof(MODULE_DIR_NAME[0]) - 1,
  kModuleSignatureDirNameLength = sizeof(MODULE_SIGNATURE_DIR_NAME)
      / sizeof(MODULE_SIGNATURE_DIR_NAME[0])
      - 1,
  kModuleConfigDirNameLength = sizeof(MODULE_CONFIG_DIR_NAME)
      / sizeof(MODULE_CONFIG_DIR_NAME[0])
      - 1,
};

static void FindModules(struct ModuleManager* module_manager) {
  BOOL is_find_next_file_success;
  BOOL is_find_close_success;

  WIN32_FIND_DATAW find_data;
  HANDLE find_file;

  if (!PathFileExistsW(module_manager->modules_dir)) {
    BOOL is_create_directory_success;

    is_create_directory_success = CreateDirectoryW(
        module_manager->modules_dir,
        NULL);
    if (!is_create_directory_success) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"CreateDirectoryW failed with error code 0x%X for directory %ls.",
          GetLastError(),
          module_manager->modules_dir);
      goto bad;
    }
    return;
  }

  if (PathIsDirectoryEmptyW(module_manager->modules_dir)) {
    return;
  }

  find_file = FindFirstFileW(module_manager->modules_dir, &find_data);
  if (find_file == INVALID_HANDLE_VALUE) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"FindFirstFileW failed with error code 0x%X for directory %ls.",
        GetLastError(),
        module_manager->modules_dir);
    goto bad;
  }

  module_manager->modules_count = 0;
  is_find_next_file_success = TRUE;
  do {
    wchar_t module_path[MAX_PATH];

    if (module_manager->modules_count >= module_manager->modules_capacity) {
      break;
    }

    PathCombineW(module_path, module_manager->modules_dir, find_data.cFileName);
    if (Module_IsValid(module_path)) {
      module_manager->modules[module_manager->modules_count] = Module_Init(
          module_path);
      ++(module_manager->modules_count);
    }
  } while (is_find_next_file_success = FindNextFileW(find_file, &find_data));

  if (!is_find_next_file_success) {
    DWORD last_error;

    last_error = GetLastError();
    if (last_error != ERROR_NO_MORE_FILES) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"FindClose failed with error code 0x%X for directory %ls.",
          last_error,
          module_manager->modules_dir);
      goto close_find_file;
    }
  }

  is_find_close_success = FindClose(find_file);
  if (!is_find_close_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"FindClose failed with error code 0x%X for directory %ls.",
        GetLastError(),
        module_manager->modules_dir);
    goto bad;
  }

  return;

close_find_file:
  FindClose(find_file);

bad:
  return;
}

/**
 * External
 */

const struct ModuleManager ModuleManager_kUninit = MODULE_MANAGER_UNINIT;

struct ModuleManager ModuleManager_Init(const wchar_t* dir) {
  struct ModuleManager module_manager;

  DWORD get_full_path_name_result;
  size_t file_count;
  size_t module_dir_length;

  get_full_path_name_result = GetFullPathNameW(
      dir,
      MAX_PATH,
      module_manager.root_dir,
      NULL);
  if (get_full_path_name_result > MAX_PATH) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"Absolute path of %ls exceeds MAX_PATH limit of %u characters.",
        dir,
        MAX_PATH);
    goto bad;
  }

  module_dir_length = get_full_path_name_result + kModuleDirNameLength - 1;
  if (module_dir_length >= MAX_PATH) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"Absolute path of %ls/"
            MODULE_DIR_NAME
            L" exceeds MAX_PATH limit of %u characters.",
        dir,
        MAX_PATH);
    goto bad;
  }

  PathCombineW(
      module_manager.modules_dir,
      module_manager.root_dir,
      MODULE_DIR_NAME);
  file_count = File_CountFromDirectory(
      module_manager.modules_dir,
      &Module_IsValid);
  module_manager.modules = malloc(
      file_count * sizeof(module_manager.modules[0]));
  if (module_manager.modules == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"malloc failed.");
    goto bad;
  }

  if (module_dir_length + kModuleConfigDirNameLength >= MAX_PATH) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"Absolute path of %ls/"
            MODULE_DIR_NAME
            L"/"
            MODULE_CONFIG_DIR_NAME
            L" exceeds MAX_PATH limit of %u characters.",
        dir,
        MAX_PATH);
    goto free_module_manager_modules;
  }

  PathCombineW(
      module_manager.modules_config_dir,
      module_manager.modules_dir,
      MODULE_CONFIG_DIR_NAME);

  if (module_dir_length + kModuleSignatureDirNameLength >= MAX_PATH) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"Absolute path of %ls/"
            MODULE_DIR_NAME
            L"/"
            MODULE_SIGNATURE_DIR_NAME
            L" exceeds MAX_PATH limit of %u characters.",
        dir,
        MAX_PATH);
    goto free_module_manager_modules;
  }

  PathCombineW(
      module_manager.modules_signature_dir,
      module_manager.modules_dir,
      MODULE_SIGNATURE_DIR_NAME);

  module_manager.modules_capacity = file_count;
  module_manager.modules_count = 0;

  return module_manager;

free_module_manager_modules:
  free(module_manager.modules);

bad:
  return ModuleManager_kUninit;
}

void ModuleManager_Deinit(struct ModuleManager* module_manager) {
  ModuleManager_UnloadModules(module_manager);
  free(module_manager->modules);
}

void ModuleManager_LoadModules(struct ModuleManager* module_manager) {
  size_t i;

  FindModules(module_manager);

  for (i = 0; i < module_manager->modules_count; ++i) {
    Module_LoadVerified(
        &module_manager->modules[i],
        module_manager->modules_signature_dir);
  }

  return;

bad:
  return;
}

void ModuleManager_UnloadModules(struct ModuleManager* module_manager) {
  size_t i;

  for (i = module_manager->modules_count; i > 0; --i) {
    Module_Deinit(&module_manager->modules[i - 1]);
  }
}
