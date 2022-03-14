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

#ifndef SGD2MODULEMANAGER_FILEW_H_
#define SGD2MODULEMANAGER_FILEW_H_

#if _MSC_VER < 1600

#define SGD2MODULEMANAGER_INTERNAL_CSTR_LIT_TO_CWSTR_LIT( \
    prefix, \
    lit \
) prefix ## lit

#define SGD2MODULEMANAGER_INTERNAL_EXPAND_MACRO(prefix, x) \
    SGD2MODULEMANAGER_INTERNAL_CSTR_LIT_TO_CWSTR_LIT(prefix, x)

#define __FILEW__ SGD2MODULEMANAGER_INTERNAL_EXPAND_MACRO(L, __FILE__)

#endif /* _MSC_VER < 1600 */

#endif /* SGD2MODULEMANAGER_FILEW_H_ */
