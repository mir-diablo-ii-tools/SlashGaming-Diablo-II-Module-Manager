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

#include "error.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>
#include <windows.h>

/*
 * A global message buffer is acceptable here, because the program will
 * exit immediately.
 */

#define FULL_ERROR_MESSAGE_FORMAT L"File: %ls\n" \
    L"Line:%u\n" \
    L"\n" \
    L"%ls"

static wchar_t format_message[Error_kMessageCapacity];
static wchar_t error_message[Error_kMessageCapacity];

/**
 * External
 */

void Error_ExitWithFormatMessage(
    const wchar_t* file,
    unsigned int line,
    const wchar_t* format,
    ...) {
  va_list vlist;

  va_start(vlist, format);
  Error_ExitWithFormatMessageV(file, line, format, vlist);
  va_end(vlist);
}

void Error_ExitWithFormatMessageV(
    const wchar_t* file,
    unsigned int line,
    const wchar_t* format,
    va_list vlist) {
  _snwprintf(
      format_message,
      Error_kMessageCapacity,
      FULL_ERROR_MESSAGE_FORMAT,
      file,
      line,
      format);
  format_message[Error_kMessageCapacity - 1] = L'\0';

  _vsnwprintf(error_message, Error_kMessageCapacity, format_message, vlist);
  error_message[Error_kMessageCapacity - 1] = L'\0';

  MessageBoxW(NULL, error_message, L"Error", MB_OK);

  exit(-1);
}
