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

#ifndef SGD2MODULEMANAGER_MODULE_H_
#define SGD2MODULEMANAGER_MODULE_H_

#include <wchar.h>
#include <windows.h>

#define MODULE_EXTENSION L".dll"
#define MODULE_SIGNATURE_EXTENSION L".sig"

enum {
  Module_kExtensionLength = sizeof(MODULE_EXTENSION)
      / sizeof(MODULE_EXTENSION[0]) - 1,
  Module_kSignatureExtensionLength = sizeof(MODULE_SIGNATURE_EXTENSION)
      / sizeof(MODULE_SIGNATURE_EXTENSION[0]) - 1,
};

struct Module {
  wchar_t path[MAX_PATH];
  HMODULE handle;
};

struct Module Module_Init(const wchar_t* path);

void Module_Deinit(struct Module* module);

void Module_Load(struct Module* module);

int Module_LocateSignature(
    struct Module* module,
    wchar_t* path,
    const wchar_t* signatures_dir);

int Module_IsValid(const wchar_t* path);

int Module_IsSignatureValid(const wchar_t* path);

#endif /* SGD2MODULEMANAGER_MODULE_H_ */
