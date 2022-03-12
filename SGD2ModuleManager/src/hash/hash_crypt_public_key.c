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

#include "hash_crypt_public_key.h"

#include <windows.h>

#include "../error.h"
#include "../filew.h"
#include "hash_crypt_provider.h"

static HCRYPTKEY global_crypt_public_key;
static int is_global_crypt_public_key_init = 0;

static const unsigned char kPublicKeyBytes[] = {
  0
};

/**
 * External
 */

void Hash_GlobalCryptPublicKey_Init(void) {
  BOOL is_crypt_import_key_success;

  HCRYPTPROV crypt_provider;

  if (is_global_crypt_public_key_init) {
    return;
  }

  crypt_provider = Hash_GlobalCryptProvider_Get();
  is_crypt_import_key_success = CryptImportKey(
      crypt_provider,
      kPublicKeyBytes,
      sizeof(kPublicKeyBytes),
      0,
      0,
      &global_crypt_public_key);
  if (!is_crypt_import_key_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptImportKey failed with error code 0x%X",
        GetLastError());
    goto bad;
  }

  is_global_crypt_public_key_init = 1;
  return;

bad:
  return;
}

void Hash_GlobalCryptPublicKey_Deinit(void) {
  BOOL is_crypt_destroy_key_success;

  if (!is_global_crypt_public_key_init) {
    return;
  }

  is_crypt_destroy_key_success = CryptDestroyKey(global_crypt_public_key);
  if (!is_crypt_destroy_key_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptDestroyKey failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  is_global_crypt_public_key_init = 0;
  return;

bad:
  return;
}

HCRYPTKEY Hash_GlobalCryptPublicKey_Get(void) {
  return global_crypt_public_key;
}
