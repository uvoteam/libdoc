SET(CMAKE_C_COMPILER /usr/bin/cc)
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wunused-variable -g")

add_library(doc
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
