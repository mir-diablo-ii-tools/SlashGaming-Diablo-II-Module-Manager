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

#include "config_struct.h"

#include <wchar.h>

#include "config_ini.h"

/**
 * External
 */

struct Config Config_Init(const wchar_t* path) {
  struct Config config;

  config = ConfigIni_Read(path);

  return config;
}

void Config_Deinit(struct Config* config) {
}

void Config_Write(const struct Config* config, const wchar_t* path) {
  ConfigIni_Write(config, path);
}
