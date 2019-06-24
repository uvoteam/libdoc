libdoc is the thread-safe library to extract text from microsoft doc files.
It is based on catdoc sources, but it is does not use any external files like codeset, configs, replacements and so on.
You don`t need to install catdoc to use this library.

Usage is simple:

#include "libdoc.h"

int doc2text(const char *buf, size_t size, char **buffer_out);

Get input buf, size of this buf, result is put in buffer_out.
Then you should free buffer_out.
return: -1 on error(errno is set), 0 on success

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
