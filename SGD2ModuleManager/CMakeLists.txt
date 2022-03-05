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

set(SOURCE_FILES
    ${RESOURCE_FILES}

    "src/dll_main.c"
)

# Output DLL
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
