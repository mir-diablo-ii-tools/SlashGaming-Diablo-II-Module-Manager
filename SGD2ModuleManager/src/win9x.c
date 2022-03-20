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

#include "win9x.h"

#include <windows.h>

#include "error.h"
#include "filew.h"

static OSVERSIONINFOW global_os_version_info;

static void InitGlobalOsVersionInfo() {
  static int is_init = 0;

  BOOL is_get_version_success;

  if (is_init) {
    return;
  }

  global_os_version_info.dwOSVersionInfoSize = sizeof(global_os_version_info);
  is_get_version_success = GetVersionExW(&global_os_version_info);
  if (!is_get_version_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"GetVersionExW failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  is_init = 1;
  return;

bad:
  return;
}

/**
 * External
 */

int Win9x_IsRunningOs(void) {
  InitGlobalOsVersionInfo();

  return global_os_version_info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
}
