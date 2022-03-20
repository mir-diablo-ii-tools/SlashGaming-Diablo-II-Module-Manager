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

#include <windows.h>

#include "error.h"
#include "filew.h"

BOOL WINAPI DllMain(HINSTANCE dll_handle, DWORD reason, LPVOID reserved) {
  switch (reason) {
    case DLL_PROCESS_ATTACH: {
      BOOL is_disable_thread_library_calls_success;

      is_disable_thread_library_calls_success = DisableThreadLibraryCalls(
          (HMODULE)dll_handle);
      if (!is_disable_thread_library_calls_success) {
        Error_ExitWithFormatMessage(
            __FILEW__,
            __LINE__,
            L"DisableThreadLibraryCalls failed with error code 0x%X.",
            GetLastError());
        goto bad;
      }
      break;
    }

    default: {
      break;
    }
  }

  return TRUE;

bad:
  return FALSE;
}
