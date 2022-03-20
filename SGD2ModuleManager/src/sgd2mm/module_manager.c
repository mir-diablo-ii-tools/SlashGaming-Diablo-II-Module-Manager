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

#include "../../include/sgd2mm/module_manager.h"

#include <stddef.h>
#include <windows.h>

#include "../config/config_global.h"
#include "../config/config_struct.h"
#include "../hash/hash_crypt_provider.h"
#include "../hash/hash_crypt_public_key.h"
#include "../module_manager/module_manager_global.h"
#include "../module_manager/module_manager_struct.h"

/**
 * External
 */

void Sgd2mm_Init(void) {
  global_config = Config_Init(GLOBAL_CONFIG_PATH);
  global_module_manager = ModuleManager_Init(MODULE_MANAGER_DIR);
  Hash_GlobalCryptProvider_Init();
  Hash_GlobalCryptPublicKey_Init();

  ModuleManager_LoadModules(&global_module_manager);
}

void Sgd2mm_Deinit(void) {
  ModuleManager_UnloadModules(&global_module_manager);

  Hash_GlobalCryptPublicKey_Deinit();
  Hash_GlobalCryptProvider_Deinit();
  ModuleManager_Deinit(&global_module_manager);
  Config_Deinit(&global_config);
}

size_t Sgd2mm_GetModulesFunctions(
    FARPROC* functions,
    const char* exported_name) {
  size_t i;
  size_t count;

  count = 0;
  for (i = 0; i < global_module_manager.modules_count; ++i) {
    functions[i] = GetProcAddress(
        global_module_manager.modules[i].handle,
        exported_name);
    if (functions[i] == NULL) {
      continue;
    }

    ++count;
  }

  return count;
}

size_t Sgd2mm_GetModulesCount(void) {
  return global_module_manager.modules_count;
}
