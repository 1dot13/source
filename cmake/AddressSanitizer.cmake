# AddressSanitizer wiring, kept in one place. Include this early: it declares
# the option and the _asan generator expression that the CRT selection in the
# root CMakeLists depends on. The two functions are ordered hook points the
# root calls at the right moments:
#   ja2_asan_instrument_first_party() — after the vendored add_subdirectory()s
#     (they must keep default flags), before our own targets are defined.
#   ja2_asan_link_binkw32_stub(<exe>) — per executable, links the Bink stubs in.

option(ADDRESS_SANITIZER "Enable AddressSanitizer for Debug and RelWithDebInfo" OFF)

# asan only on the debuggable configs, shared by the flag and the CRT choice
set(_asan "$<AND:$<BOOL:${ADDRESS_SANITIZER}>,$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>>")

if(ADDRESS_SANITIZER)
  message(STATUS "AddressSanitizer ENABLED for Debug and RelWithDebInfo (first-party code only)")
  message(STATUS "  Bink stubbed into the exe; no binkw32.dll import, so the asan exe runs from gamedir untouched")
  # Disable STL container-overflow annotations (_DISABLE_STL_ANNOTATION).
  # RakNet ships prebuilt and un-annotated; leaving annotations on makes the
  # linker reject the mix (LNK2038: mismatch detected for 'annotate_vector').
  # When every vendored C++ lib is built from our source, drop this and add
  # _ANNOTATE_STL to them instead — that restores detection without needing
  # to instrument their code.
  add_compile_definitions("$<${_asan}:_DISABLE_STL_ANNOTATION>")
  # retail binkw32.dll cannot load in an asan process (image base 0x30000000 is
  # the 32-bit shadow). __RADINEXE__ makes bink.h declare the Bink functions as
  # in-exe calls instead of dllimports (sgp/RAD.H), so ja2_asan_link_binkw32_stub
  # can satisfy them from sgp/binkw32_stub.c and the exe imports no binkw32.dll.
  # Configure-time, not per-config: it must stay in lockstep with dropping the
  # binkw32 import library from the link (see root CMakeLists).
  add_compile_definitions(__RADINEXE__)
endif()

function(ja2_asan_instrument_first_party)
  # instrument first-party code; /bigobj for the TUs asan inflates past the COFF
  # section cap (LanguageStrings.cpp). The ignorelist opts individual functions
  # with 32-bit inline __asm out of instrumentation (see cmake/asan-ignorelist.txt).
  # Empty when the option is off.
  add_compile_options(
    "$<${_asan}:-fsanitize=address;-fsanitize-ignorelist=${CMAKE_SOURCE_DIR}/cmake/asan-ignorelist.txt;/bigobj>")

  # clang-cl/lld-link do not infer the asan runtime; /WHOLEARCHIVE is invalid for llvm-lib
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_link_options(
      "$<$<AND:${_asan},$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>>:clang_rt.asan_dynamic-i386.lib;/WHOLEARCHIVE:clang_rt.asan_static_runtime_thunk-i386.lib>")
  endif()
endfunction()

function(ja2_asan_link_binkw32_stub exe)
  # Compile the no-op Bink exports into the exe. With __RADINEXE__ above the game
  # calls them directly, so this replaces the binkw32.dll the retail import
  # library would have pulled in.
  if(ADDRESS_SANITIZER)
    target_sources(${exe} PRIVATE "${CMAKE_SOURCE_DIR}/sgp/binkw32_stub.c")
  endif()
endfunction()
