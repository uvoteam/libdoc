cmake_minimum_required(VERSION 3.10)
SET(CMAKE_C_COMPILER /usr/bin/cc)
set(PROGRAM_NAME doc)
set(LIB_VERSION_STRING 1.0.0)
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wunused-variable -O2")

add_library(${PROGRAM_NAME} SHARED
  analyze.c
  catdoc.c
  catdoc.h
  charsets.c
  numutils.c
  ole.c
  ole.h
  reader.c
  replacements.c
  rtfread.c
  source_charsets.c
  spec_chars.c
  substmap.c
  writer.c
  )

set_target_properties(${PROGRAM_NAME} PROPERTIES VERSION ${LIB_VERSION_STRING})
install(TARGETS ${CONFIG_LIB_NAME}
        LIBRARY DESTINATION lib COMPONENT libraries)
