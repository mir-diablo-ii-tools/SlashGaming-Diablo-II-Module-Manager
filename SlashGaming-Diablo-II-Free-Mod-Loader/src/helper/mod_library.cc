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

#include "mod_library.hpp"

namespace sgd2fml {

ModLibrary::ModLibrary(const std::filesystem::path& library_path) :
    library_path_(library_path),
    module_handle_(LoadLibraryW(library_path_.wstring().data())) {
}

ModLibrary::ModLibrary(ModLibrary&& mod_library) noexcept :
    library_path_(std::move(mod_library.library_path_)),
    module_handle_(std::move(mod_library.module_handle_)) {
  mod_library.library_path_ = std::filesystem::path();
  mod_library.module_handle_ = nullptr;
}

ModLibrary::~ModLibrary() {
  if (this->module_handle_ != nullptr) {
    FreeLibrary(this->module_handle_);
  }
}

ModLibrary& ModLibrary::operator=(ModLibrary&& mod_library) noexcept {
  library_path_ = std::move(mod_library.library_path_);
  module_handle_ = std::move(mod_library.module_handle_);

  mod_library.library_path_ = std::filesystem::path();
  mod_library.module_handle_ = nullptr;

  return *this;
}

const std::filesystem::path& ModLibrary::library_path() const noexcept {
  return this->library_path_;
}

HMODULE ModLibrary::module_handle() const noexcept {
  return this->module_handle_;
}

bool operator==(
    const ModLibrary& lhs,
    const ModLibrary& rhs
) noexcept {
  return lhs.library_path() == rhs.library_path()
      && lhs.module_handle() == rhs.module_handle();
}

std::strong_ordering operator<=>(
    const ModLibrary& lhs,
    const ModLibrary& rhs
) noexcept {
  std::strong_ordering library_path_cmp_result =
      lhs.library_path() <=> rhs.library_path();

  if (library_path_cmp_result != std::strong_ordering::equal) {
    return library_path_cmp_result;
  }

  return lhs.module_handle() <=> rhs.module_handle();
}

} // namespace sgd2fml
