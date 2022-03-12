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

#include "hash_crypt_provider.h"

#include <stddef.h>
#include <windows.h>

#include "../error.h"
#include "../filew.h"

static HCRYPTPROV global_crypt_provider;
static int is_global_crypt_provider_init = 0;

/**
 * External
 */

void Hash_GlobalCryptProvider_Init(void) {
  BOOL is_crypt_acquire_context_success;

  if (is_global_crypt_provider_init) {
    return;
  }

  /*
   * ANSI variant used here, because unicows doesn't implement wide
   * variants of Cryptography functions.
   */
  is_crypt_acquire_context_success = CryptAcquireContextA(
      &global_crypt_provider,
      NULL,
      NULL,
      PROV_RSA_FULL,
      CRYPT_VERIFYCONTEXT);
  if (!is_crypt_acquire_context_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptAcquireContextA failed with error code 0x%X",
        GetLastError());
    goto bad;
  }

  is_global_crypt_provider_init = 1;
  return;

bad:
  return;
}

void Hash_GlobalCryptProvider_Deinit(void) {
  BOOL is_crypt_release_context_success;

  if (!is_global_crypt_provider_init) {
    return;
  }

  is_crypt_release_context_success = CryptReleaseContext(
      global_crypt_provider,
      0);
  if (!is_crypt_release_context_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptReleaseContext failed with error code 0x%X",
        GetLastError());
    goto bad;
  }

  is_global_crypt_provider_init = 0;
  return;

bad:
  return;
}

HCRYPTPROV Hash_GlobalCryptProvider_Get(void) {
  return global_crypt_provider;
}
