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

#include "file_content.h"

#include <stddef.h>
#include <wchar.h>
#include <windows.h>
#include <shlwapi.h>

#include "../error.h"
#include "../filew.h"

/**
 * External
 */

size_t File_GetSize(const wchar_t* path) {
  BOOL is_close_handle_success;

  HANDLE file;
  DWORD file_size;

  file = CreateFileW(path, 0, 0, NULL, 0, 0, NULL);
  if (file == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CreateFileW failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  file_size = GetFileSize(file, NULL);
  if (file_size == 0xFFFFFFFF) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"GetFileSize failed with error code 0x%X.",
        GetLastError());
    goto close_file;
  }

  is_close_handle_success = CloseHandle(file);
  if (!is_close_handle_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CloseHandle failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return file_size;

close_file:
  CloseHandle(file);

bad:
  return 0;
}

void File_ReadContent(
    const wchar_t* path,
    void* content,
    size_t content_size) {
  BOOL is_read_file_success;
  BOOL is_close_handle_success;

  HANDLE file;
  DWORD bytes_read_count;

  file = CreateFileW(
      path,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL);
  if (file == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CreateFileW failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  is_read_file_success = ReadFile(
      file,
      content,
      content_size,
      &bytes_read_count,
      NULL);
  if (!is_read_file_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"ReadFile failed with error code 0x%X.",
        GetLastError());
    goto close_file;
  }

  is_close_handle_success = CloseHandle(file);
  if (!is_close_handle_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CloseHandle failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return;

close_file:
  CloseHandle(file);

bad:
  return;
}

void File_WriteContent(
    const wchar_t* path,
    const void* content,
    size_t content_size) {
  BOOL is_write_file_success;
  BOOL is_close_handle_success;

  HANDLE file;
  DWORD bytes_written_count;

  file = CreateFileW(
      path,
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL);
  if (file == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CreateFileW failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  is_write_file_success = WriteFile(
      file,
      content,
      content_size,
      &bytes_written_count,
      NULL);
  if (!is_write_file_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"WriteFile failed with error code 0x%X.",
        GetLastError());
    goto close_file;
  }

  is_close_handle_success = CloseHandle(file);
  if (!is_close_handle_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CloseHandle failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return;

close_file:
  CloseHandle(file);

bad:
  return;
}
