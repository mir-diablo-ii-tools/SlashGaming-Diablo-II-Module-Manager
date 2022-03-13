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

#include "module_manager_internal.h"

#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include "config.h"
#include "error.h"
#include "filew.h"
#include "file/file_count.h"
#include "file/file_list.h"
#include "module.h"

#define MODULE_MANAGER_DIR L"./SGD2ModuleManager"

#define MODULE_DIR_NAME L"Module"
#define MODULE_DIR MODULE_MANAGER_DIR L"/" MODULE_DIR_NAME

#define MODULE_SIGNATURE_DIR_NAME L"Signature"
#define MODULE_SIGNATURE_DIR MODULE_MANAGER_DIR L"/" MODULE_SIGNATURE_DIR_NAME

#define MODULE_CONFIG_DIR_NAME L"Config"
#define MODULE_CONFIG_DIR MODULE_MANAGER_DIR L"/" MODULE_CONFIG_DIR_NAME

struct ModuleManager {
  struct Module* modules;
  size_t modules_capacity;
  size_t modules_count;
};

static struct ModuleManager global_module_manager;

static void InitGlobalModuleManager(void) {
  static int is_init = 0;

  size_t file_count;

  if (is_init) {
    return;
  }

  file_count = File_CountFromDirectory(
      MODULE_DIR,
      &Module_IsValid);
  global_module_manager.modules = malloc(
      file_count * sizeof(global_module_manager.modules[0]));
  if (global_module_manager.modules == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"malloc failed.");
    goto bad;
  }

  global_module_manager.modules_capacity = file_count;
  global_module_manager.modules_count = 0;

  is_init = 1;
  return;

bad:
  return;
}

static void FindModules(
    struct ModuleManager* module_manager,
    const wchar_t* modules_dir) {
  BOOL is_find_next_file_success;
  BOOL is_find_close_success;

  WIN32_FIND_DATAW find_data;
  HANDLE find_file;

  if (!PathFileExistsW(modules_dir)) {
    BOOL is_create_directory_success;

    is_create_directory_success = CreateDirectoryW(modules_dir, NULL);
    if (!is_create_directory_success) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"CreateDirectoryW failed with error code 0x%X for directory %ls.",
          GetLastError(),
          modules_dir);
      goto bad;
    }
    return;
  }

  if (PathIsDirectoryEmptyW(modules_dir)) {
    return;
  }

  find_file = FindFirstFileW(modules_dir, &find_data);
  if (find_file == INVALID_HANDLE_VALUE) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"FindFirstFileW failed with error code 0x%X for directory %ls.",
        GetLastError(),
        modules_dir);
    goto bad;
  }

  module_manager->modules_count = 0;
  is_find_next_file_success = TRUE;
  do {
    wchar_t module_path[MAX_PATH];

    if (module_manager->modules_count >= module_manager->modules_capacity) {
      break;
    }

    PathCombineW(module_path, modules_dir, find_data.cFileName);
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
          modules_dir);
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
        modules_dir);
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

void ModuleManager_LoadModules(void) {
  size_t i;

  InitGlobalModuleManager();

  FindModules(&global_module_manager, MODULE_DIR);

  for (i = 0; i < global_module_manager.modules_count; ++i) {
    Module_LoadVerified(
        &global_module_manager.modules[i],
        MODULE_SIGNATURE_DIR);
  }

  return;

bad:
  return;
}

void ModuleManager_UnloadModules(void) {
  size_t i;

  for (i = global_module_manager.modules_count; i > 0; --i) {
    Module_Deinit(&global_module_manager.modules[i - 1]);
  }
}