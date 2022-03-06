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

#include "license.h"

#include <stddef.h>
#include <stdio.h>

#define CONCAT2(A, B) A##B
#define CONCAT(A, B) CONCAT2(A, B)

#define CHAR_PREFIX(STR_LITERAL) STR_LITERAL
#define WCHAR_PREFIX(STR_LITERAL) CONCAT(L, STR_LITERAL)

#define LICENSE_TEXT(PREFIX_MACRO) \
    { \
      PREFIX_MACRO("SlashGaming Diablo II Module Manager"), \
      PREFIX_MACRO("Copyright (C) 2020-2022  Mir Drualga"), \
      PREFIX_MACRO(""), \
      PREFIX_MACRO("This program is free software: you can redistribute it and/or modify"), \
      PREFIX_MACRO("it under the terms of the GNU Lesser General Public License as"), \
      PREFIX_MACRO("published by the Free Software Foundation, either version 3 of the"), \
      PREFIX_MACRO("License, or (at your option) any later version."), \
      PREFIX_MACRO(""), \
      PREFIX_MACRO("This program is distributed in the hope that it will be useful, but"), \
      PREFIX_MACRO("WITHOUT ANY WARRANTY; without even the implied warranty of"), \
      PREFIX_MACRO("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU"), \
      PREFIX_MACRO("Lesser General Public License for more details."), \
      PREFIX_MACRO(""), \
      PREFIX_MACRO("You should have received a copy of the GNU Lesser General Public"), \
      PREFIX_MACRO("License along with this program. If not, see"), \
      PREFIX_MACRO("<https://www.gnu.org/licenses/>."), \
    }

static const char* const kLicenseText[] = LICENSE_TEXT(CHAR_PREFIX);

enum {
  kLicenseTextCount = sizeof(kLicenseText) / sizeof(kLicenseText),
};

/**
 * External
 */

void License_PrintText(void) {
  size_t i;

  for (i = 0; i < kLicenseTextCount; ++i) {
    puts(kLicenseText[i]);
  }
}
