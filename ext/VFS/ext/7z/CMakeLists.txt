
## 7-zip

if(WIN32)
  option(BFVFS_7ZIP_MT "multi-threading support" OFF)
  mark_as_advanced(BFVFS_7ZIP_MT)
endif()

set(INCLUDE_7z
  src/7z.h
  src/7zBuf.h
  src/7zCrc.h
  src/7zFile.h
  src/7zVersion.h
  src/Alloc.h
  src/Bcj2.h
  src/Bra.h
  src/CpuArch.h
  src/Delta.h
  src/LzFind.h
  src/LzHash.h
  src/Lzma2Dec.h
  src/Lzma2Enc.h
  src/Lzma86.h
  src/LzmaEnc.h
  src/LzmaLib.h
  src/Ppmd.h
  src/Sha256.h
  src/Types.h
#  src/Xz.h
#  src/XzEnc.h
  src/7zAlloc.h
)
if(WIN32 AND BFVFS_7ZIP_MT)
  set(INCLUDE_7z ${INCLUDE_7z}
    src/LzFindMt.h
    src/MtCoder.h
    src/Threads.h
  )
endif()

set(SOURCE_7z
  src/7zBuf.c
  src/7zBuf2.c
  src/7zCrc.c
  src/7zCrcOpt.c
  src/7zDec.c
  src/7zFile.c
  src/7zIn.c
  src/7zStream.c
  src/Alloc.c
  src/Bcj2.c
  src/Bra.c
  src/Bra86.c
  src/CpuArch.c
  src/Delta.c
  src/LzFind.c
  src/Lzma2Dec.c
  src/Lzma86Dec.c
  src/Lzma86Enc.c
  src/LzmaDec.c
  src/LzmaEnc.c
  src/Sha256.c
#  src/Xz.c
#  src/XzDec.c
#  src/XzEnc.c
#  src/XzIn.c
  src/7zAlloc.c
)
if(WIN32 AND BFVFS_7ZIP_MT)
  set(SOURCE_7z ${SOURCE_7z}
    src/LzFindMt.c
    src/Lzma2Enc.c
    src/MtCoder.c
    src/Threads.c    
  )
endif()

source_group("" FILES ${INCLUDE_7z} ${SOURCE_7z})

####################################################

if(BUILD_BFVFS_7ZIP_SHARED)
  set(BUILD_SHARED_LIBS ON)
  add_definitions(-DSZIP_EXPORT)
  set(DEFINITIONS ${DEFINITIONS} -DSZIP_IMPORT)
else()
  set(BUILD_SHARED_LIBS OFF)
  add_definitions(-DSZIP_STATIC)
  set(DEFINITIONS ${DEFINITIONS} -DSZIP_STATIC)
endif()

if(NOT WIN32 OR NOT BFVFS_7ZIP_MT)
  add_definitions(-D_7ZIP_ST)
endif()

set(BFVFS_7ZIP_TARGET "7z" CACHE INTERNAL "")

add_library( ${BFVFS_7ZIP_TARGET} ${INCLUDE_7z} ${SOURCE_7z} )

set_property(
  TARGET ${BFVFS_7ZIP_TARGET}
  PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${BFVFS_LIBRARY_DIRS}
)
set_property(
  TARGET ${BFVFS_7ZIP_TARGET}
  PROPERTY LIBRARY_OUTPUT_DIRECTORY ${BFVFS_LIBRARY_DIRS}
)
set_property(
  TARGET ${BFVFS_7ZIP_TARGET}
  PROPERTY RUNTIME_OUTPUT_DIRECTORY ${BFVFS_RUNTIME_DIRS}
)
####################################################

set(BFVFS_7ZIP_DEFINITIONS ${DEFINITIONS} CACHE INTERNAL "")
