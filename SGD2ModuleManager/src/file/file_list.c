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

#include "file_list.h"

#include <stddef.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include "../error.h"
#include "../filew.h"

/**
 * External
 */

size_t File_ListPathsFromDirectory(
    wchar_t** paths,
    size_t paths_capacity,
    const wchar_t* directory,
    int (*validity_func)(const wchar_t* path)) {
  BOOL is_find_next_file_success;
  BOOL is_find_close_success;

  size_t paths_count;

  WIN32_FIND_DATAW find_data;
  HANDLE find_file;

  if (!PathFileExistsW(directory)) {
    BOOL is_create_directory_success;

    is_create_directory_success = CreateDirectoryW(directory, NULL);
    if (!is_create_directory_success) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"CreateDirectoryW failed with error code 0x%X for directory %ls.",
          GetLastError(),
          directory);
      goto bad;
    }
    return 0;
  }

  if (PathIsDirectoryEmptyW(directory)) {
    return 0;
  }

  find_file = FindFirstFileW(directory, &find_data);
  if (find_file == INVALID_HANDLE_VALUE) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"FindFirstFileW failed with error code 0x%X for directory %ls.",
        GetLastError(),
        directory);
    goto bad;
  }

  paths_count = 0;
  is_find_next_file_success = TRUE;
  do {
    if (paths_count >= paths_capacity) {
      break;
    }

    PathCombineW(paths[paths_count], directory, find_data.cFileName);
    if (validity_func(paths[paths_count])) {
      ++paths_count;
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
          directory);
      goto bad;
    }
  }

  is_find_close_success = FindClose(find_file);
  if (!is_find_close_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"FindClose failed with error code 0x%X for directory.",
        GetLastError(),
        directory);
    goto bad;
  }

  return paths_count;

bad:
  return 0;
}
