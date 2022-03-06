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

#ifndef SGD2MODULEMANAGER_CONFIG_CONFIG_INI_H_
#define SGD2MODULEMANAGER_CONFIG_CONFIG_INI_H_

#include <wchar.h>

#include "config_struct.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct Config ConfigIni_Read(const wchar_t* path);

void ConfigIni_Write(const struct Config* config, const wchar_t* path);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* SGD2MODULEMANAGER_CONFIG_CONFIG_INI_H_ */
