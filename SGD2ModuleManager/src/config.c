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

#include "config.h"

#include "config/config_ini.h"
#include "config/config_struct.h"

#define GLOBAL_CONFIG_PATH L"./SGD2ModuleManager.ini"

static struct Config global_config;

static void InitGlobalConfig(void) {
  static int is_init = 0;

  if (is_init) {
    return;
  }

  global_config = ConfigIni_Read(GLOBAL_CONFIG_PATH);
  is_init = 1;
}

/**
 * External
 */

struct Config* Config_Get(void) {
  InitGlobalConfig();

  return &global_config;
}

void Config_Write(void) {
  InitGlobalConfig();

  ConfigIni_Write(&global_config, GLOBAL_CONFIG_PATH);
}
