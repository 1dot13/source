
## Core

##
set(INCLUDE_Core_root
  ${MOD_INCLUDE}/vfs.h
  ${MOD_INCLUDE}/vfs_debug.h
  ${MOD_INCLUDE}/vfs_file_raii.h
  ${MOD_INCLUDE}/vfs_init.h
  ${MOD_INCLUDE}/vfs_os_functions.h
  ${MOD_INCLUDE}/vfs_path.h
  ${MOD_INCLUDE}/vfs_profile.h
  ${MOD_INCLUDE}/vfs_string.h
  ${MOD_INCLUDE}/vfs_types.h
  ${MOD_INCLUDE}/vfs_vfile.h
  ${MOD_INCLUDE}/vfs_vloc.h
)
set(SOURCE_Core_root
  ${MOD_SOURCE}/vfs.cpp
  ${MOD_SOURCE}/vfs_debug.cpp
  ${MOD_SOURCE}/vfs_file_raii.cpp
  ${MOD_SOURCE}/vfs_init.cpp
  ${MOD_SOURCE}/vfs_os_functions.cpp
  ${MOD_SOURCE}/vfs_path.cpp
  ${MOD_SOURCE}/vfs_profile.cpp
  ${MOD_SOURCE}/vfs_string.cpp
  ${MOD_SOURCE}/vfs_types.cpp
  ${MOD_SOURCE}/vfs_vfile.cpp
  ${MOD_SOURCE}/vfs_vloc.cpp
)
source_group("Core" FILES ${INCLUDE_Core_root} ${SOURCE_Core_root})

##
set(INCLUDE_Core_File
  ${MOD_INCLUDE}/File/vfs_buffer_file.h
  ${MOD_INCLUDE}/File/vfs_dir_file.h
  ${MOD_INCLUDE}/File/vfs_file.h
  ${MOD_INCLUDE}/File/vfs_lib_file.h
)
set(SOURCE_Core_File
  ${MOD_SOURCE}/File/vfs_buffer_file.cpp
  ${MOD_SOURCE}/File/vfs_dir_file.cpp
  ${MOD_SOURCE}/File/vfs_file.cpp
  ${MOD_SOURCE}/File/vfs_lib_file.cpp
)
source_group("Core\\File" FILES ${INCLUDE_Core_File} ${SOURCE_Core_File})

##
set(INCLUDE_Core_Interface
  ${MOD_INCLUDE}/Interface/vfs_directory_interface.h
  ${MOD_INCLUDE}/Interface/vfs_file_interface.h
  ${MOD_INCLUDE}/Interface/vfs_iterator_interface.h
  ${MOD_INCLUDE}/Interface/vfs_library_interface.h
  ${MOD_INCLUDE}/Interface/vfs_location_interface.h
)
set(SOURCE_Core_Interface  
  ${MOD_SOURCE}/Interface/vfs_interface_members.cpp
)
source_group("Core\\Interface" FILES ${INCLUDE_Core_Interface} ${SOURCE_Core_Interface})

##
set(INCLUDE_Core_Location
  ${MOD_INCLUDE}/Location/vfs_directory_tree.h
  ${MOD_INCLUDE}/Location/vfs_lib_dir.h
  ${MOD_INCLUDE}/Location/vfs_uncompressed_lib_base.h
)
set(SOURCE_Core_Location
  ${MOD_SOURCE}/Location/vfs_directory_tree.cpp
  ${MOD_SOURCE}/Location/vfs_lib_dir.cpp
  ${MOD_SOURCE}/Location/vfs_uncompressed_lib_base.cpp
)
source_group("Core\\Location" FILES ${INCLUDE_Core_Location} ${SOURCE_Core_Location})

set(${mod}_files
  ${INCLUDE_Core_root}      ${SOURCE_Core_root}
  ${INCLUDE_Core_File}      ${SOURCE_Core_File}
  ${INCLUDE_Core_Interface} ${SOURCE_Core_Interface}
  ${INCLUDE_Core_Location}  ${SOURCE_Core_Location}
  CACHE INTERNAL ""
)