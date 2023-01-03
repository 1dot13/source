####################################################

set(INCLUDE_bfvfs_config
  ${BFVFS_INCLUDE_DIR}/vfs/vfs_config.h
)
source_group("" FILES ${INCLUDE_bfvfs_config})

set(BFVFS_files ${INCLUDE_bfvfs_config})


##
## iterate over bfVFS modules and add files
##
set(BFVFS_MODULES "Aspects" "Core" "Ext" "Tools")

foreach(mod ${BFVFS_MODULES})
  set( MOD_INCLUDE ${BFVFS_INCLUDE_DIR}/vfs/${mod} )
  set( MOD_SOURCE  ${mod} )

  include( ${mod}/files.cmake )
  set(BFVFS_files ${BFVFS_files} ${${mod}_files})
endforeach()

if(BFVFS_WITH_7ZIP)
  set(DEFINITIONS ${DEFINITIONS} "-DVFS_WITH_7ZIP")
  add_definitions(-DVFS_WITH_7ZIP ${BFVFS_7ZIP_DEFINITIONS})
  include_directories(${BFVFS_7ZIP_DIR}/src)
endif()

if(BFVFS_WITH_SLF)
  set(DEFINITIONS ${DEFINITIONS} "-DVFS_WITH_SLF")
  add_definitions(-DVFS_WITH_SLF)
endif()

####################################################

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(BUILD_BFVFS_SHARED)
  set(BUILD_SHARED_LIBS ON)
  add_definitions(-DVFS_EXPORT)
  set(DEFINITIONS ${DEFINITIONS} -DVFS_IMPORT)
else()
  set(BUILD_SHARED_LIBS OFF)
  add_definitions(-DVFS_STATIC)
  set(DEFINITIONS ${DEFINITIONS} -DVFS_STATIC)
endif()

set(BFVFS_TARGET "bfVFS" CACHE INTERNAL "")
add_library( ${BFVFS_TARGET} ${BFVFS_files} )

set_property(
  TARGET ${BFVFS_TARGET}
  PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${BFVFS_LIBRARY_DIRS}
)
set_property(
  TARGET ${BFVFS_TARGET}
  PROPERTY LIBRARY_OUTPUT_DIRECTORY ${BFVFS_LIBRARY_DIRS}
)
set_property(
  TARGET ${BFVFS_TARGET}
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BFVFS_RUNTIME_DIRS}
)

if(BUILD_BFVFS_SHARED)
  if(MINGW)
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-enable-auto-import")
  endif()
  
  if(BFVFS_WITH_7ZIP)
    target_link_libraries(${BFVFS_TARGET} ${BFVFS_7ZIP_TARGET})
  endif()
endif()

set(BFVFS_VFS_DEFINITIONS ${DEFINITIONS} CACHE INTERNAL "")
