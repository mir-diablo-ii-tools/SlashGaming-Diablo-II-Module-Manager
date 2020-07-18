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

#include "on_create_window_patch.hpp"

#include "../event/on_create_window.hpp"
#include "asm_x86_macro.h"

namespace sgd2fml {
namespace {

extern "C" static void __cdecl OnCreateWindow_Wrapper(HWND window_handle) {
  // Original code
  d2::d2gfx::SetWindowHandle(window_handle);

  OnCreateWindow(window_handle);
}

__declspec(naked) static void __cdecl InterceptionFunction_01() {
  ASM_X86(push ebp);
  ASM_X86(mov ebp, esp);

  ASM_X86(push eax);
  ASM_X86(push ecx);
  ASM_X86(push edx);

  ASM_X86(push eax);
  ASM_X86(call ASM_X86_FUNC(OnCreateWindow_Wrapper));
  ASM_X86(add esp, 4);

  ASM_X86(pop edx);
  ASM_X86(pop ecx);
  ASM_X86(pop eax);

  ASM_X86(leave);
  ASM_X86(ret);
}

} // namespace

OnCreateWindowPatch::OnCreateWindowPatch() :
    mapi::GamePatch(CreatePatch()) {
}

mapi::GamePatch OnCreateWindowPatch::CreatePatch() {
  d2::GameVersion running_game_version = d2::GetRunningGameVersionId();

  mapi::GameAddress patch_address = mapi::GameAddress::FromOffset(
      mapi::DefaultLibrary::kD2GFX,
      GetPatchOffset(running_game_version)
  );

  return mapi::GamePatch::MakeGameBranchPatch(
      std::move(patch_address),
      mapi::BranchType::kCall,
      &InterceptionFunction_01,
      5
  );
}

std::ptrdiff_t OnCreateWindowPatch::GetPatchOffset(
    d2::GameVersion game_version
) {
  switch (game_version) {
    case d2::GameVersion::k1_00: {
      return 0x5A70;
    }

    case d2::GameVersion::k1_02:
    case d2::GameVersion::k1_03: {
      return 0x5A60;
    }

    case d2::GameVersion::k1_05B: {
      return 0x45A9;
    }

    case d2::GameVersion::k1_09D:
    case d2::GameVersion::k1_10: {
      return 0x46C7;
    }

    case d2::GameVersion::k1_12A: {
      return 0x8B7E;
    }

    case d2::GameVersion::k1_13C: {
      return 0x87F0;
    }

    case d2::GameVersion::k1_13D: {
      return 0xB8E1;
    }

    case d2::GameVersion::kLod1_14C: {
      return 0xF2DEA;
    }

    case d2::GameVersion::kLod1_14D: {
      return 0xF5839;
    }
  }

  return 0;
}

} // namespace sgd2fml
