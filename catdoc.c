/*
  Copyright 1996-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#include "catdoc.h"

const short int const *source_charset = source_charsets[CP_1251];



/**************************************************************/
/*       Main program                                         */
/*  Processes options, reads charsets  files and substitution */
/*  maps and passes all remaining args to processfile         */
/**************************************************************/
int doc2text(const char *buf, size_t size, char **buffer_out) {
	FILE *f, *out;
	int c;
    size_t size_out;

	
	if (LINE_BUF_SIZE - LONGEST_SEQUENCE <= WRAP_MARGIN) {
        errno = EINVAL;
        return -1;
	}  
	
    c=0;
    out = open_memstream(buffer_out, &size_out);
    if (!out) return -1;
    
    f = fmemopen((char*)buf, size, "rb");
    if (!f) {
        fclose(out);
        return -1;
    }

    c = analyze_format(f, out);
    fclose(f);
	fclose(out);

	return c;
}
