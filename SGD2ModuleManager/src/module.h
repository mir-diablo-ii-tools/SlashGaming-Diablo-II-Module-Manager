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

#ifndef SGD2MODULEMANAGER_MODULE_H_
#define SGD2MODULEMANAGER_MODULE_H_

#include <wchar.h>
#include <windows.h>

struct Module {
  wchar_t path[MAX_PATH];
  HMODULE handle;
};

struct Module Module_Init(const wchar_t* path);

void Module_Deinit(struct Module* module);

void Module_Load(struct Module* module);

int Module_LoadVerified(struct Module* module, const wchar_t* signatures_dir);

int Module_LocateSignature(
    const struct Module* module,
    wchar_t* signature_path,
    const wchar_t* signatures_dir);

int Module_VerifySignature(
    const struct Module* module,
    const wchar_t* signature_path);

int Module_IsValid(const wchar_t* path);

int Module_IsSignatureValid(const wchar_t* path);

#endif /* SGD2MODULEMANAGER_MODULE_H_ */
