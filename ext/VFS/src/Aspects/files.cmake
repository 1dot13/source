
## Aspects

set(INCLUDE_Aspects
  ${MOD_INCLUDE}/vfs_logging.h
  ${MOD_INCLUDE}/vfs_settings.h
  ${MOD_INCLUDE}/vfs_synchronization.h
)
set(SOURCE_Aspects
  ${MOD_SOURCE}/vfs_logging.cpp
  ${MOD_SOURCE}/vfs_settings.cpp
  ${MOD_SOURCE}/vfs_synchronization.cpp
)
source_group( "Aspects" FILES ${INCLUDE_Aspects} ${SOURCE_Aspects} )

set(${mod}_files
  ${INCLUDE_Aspects} ${SOURCE_Aspects}
  CACHE INTERNAL ""
)
