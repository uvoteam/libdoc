libdoc is the thread-safe library to extract text from microsoft doc files.
It is based on catdoc sources, but it is does not use any external files like codeset, configs, replacements and so on.
You don`t need to install catdoc to use this library.
Lib is not well tested, so use it on your own risk.

Usage is simple:
#include "libdoc.h"

int doc2text(const char *buf, size_t size, char **buffer_out);

Get input buf, size of this buf, result is put in buffer_out.
Then you should free buffer_out.
return: -1 on error(errno is set), 0 on success

There are examples of usage in example directory.


Build:(cmake is required)
cmake .
make


Libdoc is distributed under GNU Public License version 2 or above.