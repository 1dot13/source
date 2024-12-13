set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_RC_COMPILER llvm-rc)

set(triple i386-pc-win32-msvc)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_RC_COMPILER_TARGET ${triple})

set(msvc_dir "$ENV{MSVC_DIRECTORY}")
if(msvc_dir STREQUAL "")
	message(FATAL_ERROR "Please set a valid MSVC_DIRECTORY environment variable")
endif()

# Don't remember which version of Visual Studio Build Tools this is.
# When updating this, make sure to pin the version so people can get it.
set(msvc_include
       "${msvc_dir}/14.34.31933/include"
       "${msvc_dir}/14.34.31933/ATLMFC/include"
       "${msvc_dir}/include/10.0.22000.0/ucrt"
       "${msvc_dir}/include/10.0.22000.0/um"
       "${msvc_dir}/include/10.0.22000.0/shared"
       "${msvc_dir}/include/10.0.22000.0/winrt"
       "${msvc_dir}/include/10.0.22000.0/cppwinrt"
)
set(msvc_libraries
       "${msvc_dir}/14.34.31933/ATLMFC/lib/x86"
       "${msvc_dir}/14.34.31933/lib/x86"
       "${msvc_dir}/lib/10.0.22000.0/ucrt/x86"
       "${msvc_dir}/lib/10.0.22000.0/um/x86"
)

foreach(LANG C CXX)
	set(CMAKE_${LANG}_STANDARD_INCLUDE_DIRECTORIES ${msvc_include})
endforeach()

link_directories(${msvc_libraries})
