/**
 * SlashGaming Diablo II Free Mod Loader
 * Copyright (C) 2020  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Free Mod Loader.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permissions under GNU Affero General Public License version 3
 *  section 7
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with Diablo II (or a modified version of that game and its
 *  libraries), containing parts covered by the terms of Blizzard End User
 *  License Agreement, the licensors of this Program grant you additional
 *  permission to convey the resulting work. This additional permission is
 *  also extended to any combination of expansions, mods, and remasters of
 *  the game.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any Graphics Device Interface (GDI), DirectDraw, Direct3D,
 *  Glide, OpenGL, or Rave wrapper (or modified versions of those
 *  libraries), containing parts not covered by a compatible license, the
 *  licensors of this Program grant you additional permission to convey the
 *  resulting work.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any library (or a modified version of that library) that links
 *  to Diablo II (or a modified version of that game and its libraries),
 *  containing parts not covered by a compatible license, the licensors of
 *  this Program grant you additional permission to convey the resulting
 *  work.
 */

#include "on_library_attach.hpp"

#include <cctype>

namespace sgd2fml {
namespace {

static bool LoadLibraries() {
  const std::filesystem::path current_path = std::filesystem::current_path();
  const std::filesystem::path dll_directory_path =
      current_path / "sgd2fml" / "dll";

  /* Mod directory doesn't exist, so create them and leave. */
  if (!std::filesystem::exists(dll_directory_path)
      || !std::filesystem::is_directory(dll_directory_path)) {
    std::filesystem::create_directories(dll_directory_path);

    return true;
  }

  /* Load all DLL files in the DLL directory. */
  std::filesystem::directory_iterator dll_directory_iterator(
      dll_directory_path
  );

  for (const auto& file : dll_directory_iterator) {
    if (!file.is_regular_file()) {
      continue;
    }

    /* Check case-insensitive that the file extension is dll. */
    std::string file_extension = file.path().extension().string();

    for (char& ch : file_extension) {
      ch = std::tolower(ch);
    }

    if (file_extension != ".dll") {
      continue;
    }

    ModLibrary::AddModLibrary(file);
  }

  return true;
}

} // namespace

bool OnLibraryAttach(HINSTANCE hinstDLL, EventPatches& event_patches) {
  event_patches.Apply();
  LoadLibraries();

  return true;
}

} // namespace sgd2fml
