
## Tools

set(INCLUDE_Tools
  ${MOD_INCLUDE}/vfs_allocator.h
  ${MOD_INCLUDE}/vfs_file_logger.h
  ${MOD_INCLUDE}/vfs_hp_timer.h
  ${MOD_INCLUDE}/vfs_log.h
  ${MOD_INCLUDE}/vfs_parser_tools.h
  ${MOD_INCLUDE}/vfs_profiler.h
  ${MOD_INCLUDE}/vfs_property_container.h
  ${MOD_INCLUDE}/vfs_tools.h
)
set(SOURCE_Tools
  ${MOD_SOURCE}/vfs_allocator.cpp
  ${MOD_SOURCE}/vfs_file_logger.cpp
  ${MOD_SOURCE}/vfs_hp_timer.cpp
  ${MOD_SOURCE}/vfs_log.cpp
  ${MOD_SOURCE}/vfs_parser_tools.cpp
  ${MOD_SOURCE}/vfs_profiler.cpp
  ${MOD_SOURCE}/vfs_property_container.cpp
  ${MOD_SOURCE}/vfs_tools.cpp
)
source_group(Tools FILES ${INCLUDE_Tools} ${SOURCE_Tools})

set(${mod}_files
  ${INCLUDE_Tools} ${SOURCE_Tools}
  CACHE INTERNAL ""
)
