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

#ifndef SGD2MODULEMANAGER_HASH_HASH_SHA384_H_
#define SGD2MODULEMANAGER_HASH_HASH_SHA384_H_

#include <wchar.h>

#define HASH_SHA384_SIGNATURE_FILE_EXTENSION L".sha384sig"

enum {
  HashSha384_kSignatureFileExtensionLength =
      sizeof(HASH_SHA384_SIGNATURE_FILE_EXTENSION)
          / sizeof(HASH_SHA384_SIGNATURE_FILE_EXTENSION[0])
          - 1,
  HashSha384_kSize = 48,
};

void HashSha384_GenerateFromFile(unsigned char* hash, const wchar_t* path);

int HashSha384_VerifySignatureFile(
    const wchar_t* content_path,
    const wchar_t* signature_path);

#endif /* SGD2MODULEMANAGER_HASH_HASH_SHA384_H_ */
