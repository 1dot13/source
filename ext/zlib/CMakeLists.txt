add_library(zlib
"gzread.c"
"gzlib.c"
"gzclose.c"
"crc32.c"
"zutil.c"
"uncompr.c"
"compress.c"
"inffast.c"
"inflate.c"
"adler32.c"
"infback.c"
"deflate.c"
"gzwrite.c"
"inftrees.c"
"trees.c"
)
target_include_directories(zlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
