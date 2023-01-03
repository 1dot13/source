
## Ext

if(BFVFS_WITH_7ZIP)
  set(INCLUDE_Ext_7zip
    ${MOD_INCLUDE}/7z/vfs_7z_library.h
    ${MOD_INCLUDE}/7z/vfs_create_7z_library.h
  )
  set(SOURCE_Ext_7zip
    ${MOD_SOURCE}/7z/vfs_7z_library.cpp
    ${MOD_SOURCE}/7z/vfs_create_7z_library.cpp
  )
  source_group("Ext" FILES ${INCLUDE_Ext_7zip} ${SOURCE_Ext_7zip})
endif()

if(BFVFS_WITH_SLF)
  set(INCLUDE_Ext_slf 
    ${MOD_INCLUDE}/slf/vfs_slf_library.h
  )
  set(SOURCE_Ext_slf
    ${MOD_SOURCE}/slf/vfs_slf_library.cpp
  )
  source_group("Ext" FILES ${INCLUDE_Ext_slf} ${SOURCE_Ext_slf})
endif()

set(${mod}_files
  ${INCLUDE_Ext_7zip} ${SOURCE_Ext_7zip}
  ${INCLUDE_Ext_slf}  ${SOURCE_Ext_slf}
  CACHE INTERNAL ""
)
