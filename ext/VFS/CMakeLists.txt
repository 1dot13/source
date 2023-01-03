cmake_minimum_required(VERSION 2.6)

project(bfVFS)

set(BFVFS_VERSION_MAJOR 1)
set(BFVFS_VERSION_MINOR 0)
set(BFVFS_VERSION_PATCH 0)

##
## determine input/output directories
##
if(BFVFS_INTEGRATED_SOURCE_DIR)
  set(BFVFS_SOURCE_DIR ${BFVFS_INTEGRATED_SOURCE_DIR})
else()
  set(BFVFS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(BFVFS_INTEGRATED_OUTPUT_DIR)
  set(BFVFS_OUTPUT_DIR ${BFVFS_INTEGRATED_OUTPUT_DIR})
else()
  set(BFVFS_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()


if(BFVFS_INTEGRATED_LIBRARY_DIR)
  set(BFVFS_LIBRARY_DIR ${BFVFS_INTEGRATED_LIBRARY_DIR})
else()
  set(BFVFS_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib)
endif()

if(BFVFS_INTEGRATED_RUNTIME_DIR)
  set(BFVFS_RUNTIME_DIR ${BFVFS_INTEGRATED_RUNTIME_DIR})
else()
  set(BFVFS_RUNTIME_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

set(BFVFS_INCLUDE_DIR  ${BFVFS_SOURCE_DIR}/include)

##
## set bfVFS "output" variables
##
set(BFVFS_INCLUDE_DIRS ${BFVFS_INCLUDE_DIR})
set(BFVFS_LIBRARY_DIRS ${BFVFS_LIBRARY_DIR})
set(BFVFS_RUNTIME_DIRS ${BFVFS_RUNTIME_DIR})

##
## Options
##
option(BUILD_BFVFS_SHARED "Build shared libraries" OFF)
option(BFVFS_WITH_SLF     "Enable SLF support"     ON)
option(BFVFS_WITH_7ZIP    "Enable 7-zip support"   ON)

if(BFVFS_WITH_7ZIP)
  set(BFVFS_7ZIP_DIR ${BFVFS_SOURCE_DIR}/ext/7z)
  
  if(BUILD_BFVFS_SHARED)
    set(BUILD_BFVFS_7ZIP_SHARED ON)
  endif()
endif()

##
## set include directories for projects
##
include_directories(${BFVFS_INCLUDE_DIRS}
  ${BFVFS_SOURCE_DIR}/ext/utfcpp/source
)

##
## Build 7zip (if selected)
##
if(BFVFS_WITH_7ZIP)
  add_subdirectory(${BFVFS_7ZIP_DIR})
endif()

##
## Build bfVFS
##
add_subdirectory(src)

##
## set more "output" variables
##
set(BFVFS_LIBRARIES ${BFVFS_TARGET})
if(BFVFS_WITH_7ZIP)
  set(BFVFS_LIBRARIES ${BFVFS_LIBRARIES} ${BFVFS_7ZIP_TARGET})
endif()
set(BFVFS_DEFINITIONS ${BFVFS_VFS_DEFINITIONS} ${BFVFS_7ZIP_DEFINITIONS})

##
## make bfVFS easier to use in other applications
##
configure_file(${BFVFS_SOURCE_DIR}/BFVFSConfig.cmake.in
               ${BFVFS_OUTPUT_DIR}/BFVFSConfig.cmake @ONLY IMMEDIATE)

configure_file(${BFVFS_SOURCE_DIR}/UseBFVFS.cmake.in
               ${BFVFS_OUTPUT_DIR}/UseBFVFS.cmake @ONLY IMMEDIATE)

##
## debug CMake
##
mark_as_advanced(BFVFS_DEBUG_CMAKE)
option(BFVFS_DEBUG_CMAKE OFF)
if(BFVFS_DEBUG_CMAKE)
  message("OPTIONS :")
  message("  BUILD_BFVFS_SHARED - " ${BUILD_BFVFS_SHARED})
  message("  BFVFS_WITH_7ZIP    - " ${BFVFS_WITH_7ZIP})
  message("  BFVFS_WITH_SLF     - " ${BFVFS_WITH_SLF})

  message("BFVFS_DEFINITIONS :")
  foreach(def ${BFVFS_DEFINITIONS})
    message("  " ${def})
  endforeach()
  
  message("BFVFS_INCLUDE_DIRS :")
  foreach(dir ${BFVFS_INCLUDE_DIRS})
    message("  " ${dir})
  endforeach()

  message("BFVFS_LIBRARY_DIRS :")
  foreach(dir ${BFVFS_LIBRARY_DIRS})
    message("  " ${dir})
  endforeach()

  message("BFVFS_LIBRARIES :")
  foreach(lib ${BFVFS_LIBRARIES})
    message("  " ${lib})
  endforeach()
endif()

