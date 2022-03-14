# SlashGaming Diablo II Module Manager
# Copyright (C) 2020-2022  Mir Drualga
#
# This file is part of SlashGaming Diablo II Module Manager.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program. If not, see
# <https://www.gnu.org/licenses/>.

# Name of the project, also is the name of the file
project(SGD2ModuleManager)

# Define requirements for C
set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Remove MinGW compiled binary "lib" prefix
if (MINGW)
    set(CMAKE_IMPORT_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif (MINGW)

# List all of the source files here
set(RESOURCE_FILES
)

set(CONFIG_PARSER
    "src/config/config_default.c"
    "src/config/config_default.h"

    "src/config/config_ini_key.h"
    "src/config/config_ini_reader.c"
    "src/config/config_ini_writer.c"
    "src/config/config_ini.h"

    "src/config/config_struct.h"

    "src/config.c"
    "src/config.h"
)

set(FILE_OPERATION
    "src/file/file_content.c"
    "src/file/file_content.h"
    "src/file/file_count.c"
    "src/file/file_count.h"
    "src/file/file_list.c"
    "src/file/file_list.h"
)

set(HASH
    "src/hash/hash_crypt_provider.c"
    "src/hash/hash_crypt_provider.h"
    "src/hash/hash_crypt_public_key.c"
    "src/hash/hash_crypt_public_key.h"
    "src/hash/hash_sha1.c"
    "src/hash/hash_sha1.h"
    "src/hash/hash_sha384.c"
    "src/hash/hash_sha384.h"
    "src/hash/hash_shared.c"
    "src/hash/hash_shared.h"
)

set(MODULE
    "src/module.c"
    "src/module.h"
)

set(MODULE_MANAGER
    "src/module_manager/module_manager_global.c"
    "src/module_manager/module_manager_global.h"
    "src/module_manager/module_manager_struct.c"
    "src/module_manager/module_manager_struct.h"

    "include/sgd2mm/module_manager.h"
    "src/sgd2mm/module_manager.c"
)

set(SOURCE_FILES
    ${RESOURCE_FILES}

    ${CONFIG_PARSER}
    ${FILE_OPERATION}
    ${HASH}
    ${MODULE}
    ${MODULE_MANAGER}

    "include/dllapi_define.inc"
    "include/dllapi_undefine.inc"

    "src/dll_main.c"

    "src/error.c"
    "src/error.h"

    "src/filew.h"

    "src/license.c"
    "src/license.h"

    "src/win9x.c"
    "src/win9x.h"
)

# Output DLL
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE SGD2MM_DLLEXPORT
    INTERFACE SGD2MM_DLLIMPORT)

target_include_directories(${PROJECT_NAME} PUBLIC "include")

target_link_libraries(${PROJECT_NAME} shlwapi)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
