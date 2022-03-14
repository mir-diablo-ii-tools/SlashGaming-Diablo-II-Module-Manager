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

#include "config_ini.h"

#include <wchar.h>
#include <windows.h>

#include "config_default.h"
#include "config_ini_key.h"
#include "config_struct.h"

void WriteIsOfflineMode(const struct Config* config, const wchar_t* path) {
  const wchar_t* is_offline_mode_str = config->is_offline_mode
      ? L"1"
      : L"0";

  WritePrivateProfileStringW(
      SGD2MODULEMANAGER_SECTION,
      IS_OFFLINE_MODE_KEY,
      is_offline_mode_str,
      path);
}

/**
 * External
 */

void ConfigIni_Write(const struct Config* config, const wchar_t* path) {
  WriteIsOfflineMode(config, path);
}
