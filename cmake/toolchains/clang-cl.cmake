# Usage:
# MSVC_SDK=/path/to/msvc cmake --toolchain cmake/toolchains/clang-cl.cmake ...
#
# MSVC_SDK is where the cross headers and libraries live: the directory holding
# vc/ and kits/, which for msvc-wine is the root it was installed to. It comes
# from the environment.

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86)

set(MSVC_SDK "$ENV{MSVC_SDK}")
if(NOT IS_DIRECTORY "${MSVC_SDK}/vc/tools/msvc"
   OR NOT IS_DIRECTORY "${MSVC_SDK}/kits/10/include")
  message(FATAL_ERROR
    "MSVC_SDK (${MSVC_SDK}) does not hold vc/tools/msvc and kits/10/include. "
    "Point it at the MSVC installation, e.g. MSVC_SDK=/home/you/msvc")
endif()

# Both trees are versioned directories; take the newest of each rather than
# pinning a version that goes stale the next time the toolchain is updated.
function(_newest_subdirectory result dir)
  file(GLOB versions "${dir}/*")
  list(SORT versions)
  list(POP_BACK versions newest)
  set(${result} "${newest}" PARENT_SCOPE)
endfunction()

_newest_subdirectory(_msvc "${MSVC_SDK}/vc/tools/msvc")
_newest_subdirectory(_sdk "${MSVC_SDK}/kits/10/include")
get_filename_component(_sdk_version "${_sdk}" NAME)
set(_sdk_lib "${MSVC_SDK}/kits/10/lib/${_sdk_version}")

# Find every tool here rather than trusting PATH, so the choice of compiler,
# linker and rc is the toolchain file's alone.
foreach(tool clang-cl lld-link llvm-rc llvm-lib llvm-mt llvm-dlltool)
  string(TOUPPER "${tool}" _variable)
  string(REPLACE "-" "_" _variable "${_variable}")
  # An absolute path: CMake resolves a bare name against the source directory.
  find_program(${_variable} NAMES ${tool} REQUIRED)
endforeach()

set(CMAKE_C_COMPILER "${CLANG_CL}")
set(CMAKE_CXX_COMPILER "${CLANG_CL}")
set(CMAKE_LINKER "${LLD_LINK}")
set(CMAKE_RC_COMPILER "${LLVM_RC}")
set(CMAKE_AR "${LLVM_LIB}")
set(CMAKE_MT "${LLVM_MT}")

# clang-cl does not read INCLUDE and LIB the way cl does, so every directory
# has to be named. -imsvc rather than -I marks them as system headers, which
# keeps warnings about the SDK out of the build log.
set(_includes
  "${_msvc}/atlmfc/include"
  "${_msvc}/include"
  "${_sdk}/shared"
  "${_sdk}/ucrt"
  "${_sdk}/um"
  "${_sdk}/winrt"
  "${_sdk}/km"
)

set(_flags "--target=i386-pc-windows-msvc")
foreach(directory ${_includes})
  string(APPEND _flags " -imsvc ${directory}")
endforeach()
set(CMAKE_C_FLAGS_INIT "${_flags}")
set(CMAKE_CXX_FLAGS_INIT "${_flags}")

# The resource compiler is preprocessed by clang-cl, which needs the same
# headers again: ja2.rc includes windows.h. It takes -I, not -imsvc.
set(_rc_flags "")
foreach(directory ${_includes})
  string(APPEND _rc_flags " -I ${directory}")
endforeach()
set(CMAKE_RC_FLAGS_INIT "${_rc_flags}")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "/libpath:${_msvc}/lib/x86 /libpath:${_sdk_lib}/ucrt/x86 /libpath:${_sdk_lib}/um/x86")

# lld-link cannot bind the Bink import library the game ships, so rebuild a
# bindable one from binkw32.def (which explains why) and hand it to the top-level
# CMakeLists through binkw32_lib. MSVC keeps using the shipped library.
execute_process(
  COMMAND "${LLVM_DLLTOOL}" -m i386 --no-leading-underscore
          -d "${CMAKE_CURRENT_LIST_DIR}/../../binkw32.def"
          -l "${CMAKE_BINARY_DIR}/binkw32.lib"
  RESULT_VARIABLE _binkw32DlltoolResult)
if(NOT _binkw32DlltoolResult EQUAL 0)
  message(FATAL_ERROR "llvm-dlltool failed to build the binkw32 import library")
endif()
set(binkw32_lib "${CMAKE_BINARY_DIR}/binkw32.lib")
