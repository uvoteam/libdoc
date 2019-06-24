libdoc is the thread-safe library to extract text from microsoft doc files.
It is based on catdoc sources, but it is does not use any external files like codeset, configs, replacements and so on.
You don`t need to install catdoc to use this library.

Usage is simple:

#include "libdoc.h"

int doc2text(const char *buf, size_t size, char **buffer_out);

Input arguments:
buf - binary content of .doc document,
size -  size of the content

Output:
buffer_out - pointer to extracted .doc text(zero terminated).
buffer_out is allocated inside doc2text, so you should free buffer_out after usage.

Return:
-1 on error(errno is set), 0 on success

There are examples of usage in example directory.


Build:(cmake is required)

1. mkdir build

2. cd ./build

3.cmake ..

4.make

5.make install


Python usage:

1. cd ./python

2. ./setup install


Libdoc is distributed under GNU Public License version 2 or above.
