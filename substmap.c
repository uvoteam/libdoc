/*****************************************************************/
/* Substitution maps to replace some unicode characters with     */
/* multicharacter sequences                                      */
/*                                                               */
/* This file is part of catdoc project                           */
/* (c) Victor Wagner 1998-2003, (c) Alex Ott 2003	             */
/*****************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "catdoc.h"


/******************************************************************/
/* checks for terminator of character sequence. If stop is something
 * like quote - check for same char. If it is \n, check for any space
 ********************************************************************/ 
int isstop(char  c, char stop) {
	if (stop=='\n') {
		return isspace(c);
	} else {
		return c==stop;
	}
}

/*************************************************************************/
/* searches for string in the substituton map. Returns NULL if not found */
/*************************************************************************/
const char * map_subst ( SUBSTMAP map, int uc) {
	const char (*p)[256][4]= map + ((unsigned)(uc) >>8);
    if (!(*p)[uc & 0xff][0]) return NULL;
	return (*p)[uc & 0xff];
}
