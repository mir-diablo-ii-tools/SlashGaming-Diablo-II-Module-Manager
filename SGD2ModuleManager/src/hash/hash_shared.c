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

#include "hash_shared.h"

#include <wchar.h>
#include <windows.h>

#include "../error.h"
#include "../file/file_content.h"
#include "../filew.h"
#include "hash_crypt_provider.h"
#include "hash_crypt_public_key.h"

static void HashDataFromFileContent(
    HCRYPTHASH crypt_hash,
    const wchar_t* path) {
  enum {
    kBufferSize = 256,
  };

  BOOL is_close_handle_success;

  HANDLE file;
  DWORD bytes_read_count;

  file = CreateFileW(
      path,
      GENERIC_READ,
      0,
      NULL,
      OPEN_EXISTING,
      0,
      NULL);
  if (file == NULL) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CreateFileW failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  do {
    BOOL is_read_file_success;
    BOOL is_crypt_hash_data_success;

    unsigned char buffer[kBufferSize];

    is_read_file_success = ReadFile(
        file,
        buffer,
        kBufferSize,
        &bytes_read_count,
        NULL);
    if (!is_read_file_success) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"ReadFile failed with error code 0x%X.",
          GetLastError());
      goto close_file;
    }

    is_crypt_hash_data_success = CryptHashData(
        crypt_hash,
        buffer,
        bytes_read_count,
        0);
    if (!is_crypt_hash_data_success) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"CryptHashData failed with error code 0x%X.",
          GetLastError());
      goto close_file;
    }
  } while (bytes_read_count > 0);

  is_close_handle_success = CloseHandle(file);
  if (!is_close_handle_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CloseHandle failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return;

close_file:
  CloseHandle(file);

bad:
  return;
}

static void SetUpCryptHashFromFile(
    HCRYPTHASH crypt_hash,
    ALG_ID alg,
    const wchar_t* path) {
  BOOL is_crypt_create_hash_success;

  HCRYPTPROV crypt_provider;

  crypt_provider = Hash_GlobalCryptProvider_Get();
  is_crypt_create_hash_success = CryptCreateHash(
      crypt_provider,
      alg,
      0,
      0,
      &crypt_hash);
  if (!is_crypt_create_hash_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptCreateHash failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  HashDataFromFileContent(crypt_hash, path);
  return;

bad:
  return;
}

/**
 * External
 */

void Hash_GenerateFromFile(
    unsigned char* hash,
    DWORD hash_size,
    ALG_ID alg,
    const wchar_t* path) {
  BOOL is_crypt_get_hash_param_success;
  BOOL is_crypt_destroy_hash_success;

  HCRYPTHASH crypt_hash;

  SetUpCryptHashFromFile(crypt_hash, alg, path);

  is_crypt_get_hash_param_success = CryptGetHashParam(
      crypt_hash,
      HP_HASHVAL,
      hash,
      &hash_size,
      0);
  if (!is_crypt_get_hash_param_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptGetHashParam failed with error code 0x%X.",
        GetLastError());
    goto crypt_destroy_hash;
  }

  is_crypt_destroy_hash_success = CryptDestroyHash(crypt_hash);
  if (!is_crypt_destroy_hash_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptDestroyHash failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return;

crypt_destroy_hash:
  CryptDestroyHash(crypt_hash);

bad:
  return;
}

int Hash_VerifySignatureFile(
    ALG_ID alg,
    const wchar_t* content_path,
    const wchar_t* signature_path) {
  enum {
    kKeySizeLimit = 2048,
    kSignatureSizeLimit = 1024,
  };

  BOOL is_crypt_get_hash_param_success;
  BOOL is_crypt_verify_signature_success;
  BOOL is_crypt_destroy_hash_success;

  HCRYPTHASH crypt_hash;
  size_t signature_size;
  unsigned char signature[kSignatureSizeLimit];

  SetUpCryptHashFromFile(crypt_hash, alg, content_path);

  signature_size = File_GetSize(content_path);
  if (signature_size > kSignatureSizeLimit) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"Signature size %u exceeds signature size limit of %u.",
        signature_size,
        kSignatureSizeLimit);
    goto bad;
  }

  File_ReadContent(content_path, signature, signature_size);

  /*
   * ANSI variant used here, because unicows doesn't implement wide
   * variants of Cryptography functions.
   */
  is_crypt_verify_signature_success = CryptVerifySignatureA(
      crypt_hash,
      signature,
      signature_size,
      Hash_GlobalCryptPublicKey_Get(),
      NULL,
      0);
  if (!is_crypt_verify_signature_success) {
    DWORD last_error;

    last_error = GetLastError();
    if (last_error != NTE_BAD_SIGNATURE) {
      Error_ExitWithFormatMessage(
          __FILEW__,
          __LINE__,
          L"CryptDestroyHash failed with error code 0x%X.",
          last_error);
      goto bad;
    }
  }

  is_crypt_destroy_hash_success = CryptDestroyHash(crypt_hash);
  if (!is_crypt_destroy_hash_success) {
    Error_ExitWithFormatMessage(
        __FILEW__,
        __LINE__,
        L"CryptDestroyHash failed with error code 0x%X.",
        GetLastError());
    goto bad;
  }

  return is_crypt_verify_signature_success;

crypt_destroy_hash:
  CryptDestroyHash(crypt_hash);

bad:
  return 0;
}
