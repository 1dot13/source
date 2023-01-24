add_library(libpng
"png.c"
"pngerror.c"
"pnggccrd.c"
"pngget.c"
"pngmem.c"
"pngpread.c"
"pngread.c"
"pngrio.c"
"pngrtran.c"
"pngrutil.c"
"pngset.c"
"pngtrans.c"
"pngwio.c"
"pngwrite.c"
"pngwtran.c"
"pngwutil.c"
)
target_include_directories(libpng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libpng PUBLIC zlib)
