/*
  Copyright 1998-2003 Victor Wagner
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
#include "catdoc.h"


/************************************************************************/
/* Converts char in input charset into unicode representation           */
/* Should be converted to macro                                         */
/************************************************************************/
int to_unicode (const short int *charset, int c) {
	return charset[c];
}
/************************************************************************/
/* Search inverse charset record for given unicode char and returns     */
/* 0-255 char value if found, -1 otherwise                              */
/************************************************************************/
int from_unicode (CHARSET charset, int u) {
	short int *p;
	/* This is really assignment, not comparation */
	if ((p=charset[(unsigned)u>>8])) {
		return p[u & 0xff];
	} else {
		return -1;
	}
}

/************************************************************************/
/* Reads 8-bit char and convers it from source charset                  */
/************************************************************************/

int get_8bit_char (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                   unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs)
{
	unsigned char buf;
	if (io_funcs->catdoc_read(&buf, 1, 1, f, ole_params)==0) return EOF;
	(*offset)++;  
	return to_unicode(source_charset,buf);
}


/************************************************************************/
/* Reads 16-bit unicode value. MS-Word runs on LSB-first machine only,  */
/* so read lsb first always and don't care about proper bit order       */
/************************************************************************/

int get_utf16lsb (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                  unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs) {
	unsigned char buf[2];
    int result;

	result=io_funcs->catdoc_read(buf, 1, 2, f, ole_params);
	if (result<0) return -1;

	if (result !=2) {
		return EOF;
	}	
	(*offset)+=2;
	return ((int)buf[1])|(((int)buf[0])<<8);
}

/************************************************************************/
/* Reads 16-bit unicode value written in MSB order. For processing 
 * non-word files            .                                          */
/************************************************************************/
int get_utf16msb (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                  unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs) {
	unsigned char buf[2];
    int result;
	result=io_funcs->catdoc_read(buf, 1, 2, f, ole_params);
	if (result<0) return -1;

	if (result !=2) {
		return EOF;
	}	
	(*offset)+=2;
	return ((int)buf[0])|(((int)buf[1])<<8);
}

int get_utf8 (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params, unsigned char read_buf[256],
              int *buf_is_unicode, struct io_funcs_t *io_funcs) {
	unsigned char buf[3];
	int d,c;
    int result;
	result=io_funcs->catdoc_read(buf, 1, 1, f, ole_params);
	if (result<0) return -1;

	if (result==0) return EOF;
	c=buf[0];
	d=0;
	if (c<0x80) 
		return c;
	if (c <0xC0) 
		return 0xfeff; /*skip corrupted sequebces*/
	if (c <0xE0) {
		if (io_funcs->catdoc_read(buf+1, 1, 1, f, ole_params)<=0) return EOF;
		return ((c & 0x1F)<<6 | ((char)buf[1] & 0x3F));
	}
	if (c <0xF0) {
		if (io_funcs->catdoc_read(buf+1, 1, 2, f, ole_params)<=2) return (int)EOF;
		return ((c & 0x0F)<<12)|
			((buf[1] & 0x3f)<<6)|
					 (buf[2] & 0x3f);
	}  
	return 0xFEFF; 
}

/**************************************************************************/
/*  Converts unicode char to output charset sequence. Coversion have      */
/*  three steps: 1. Replacement map is searched for the character in case */
/* it is not allowed for output format (% in TeX, < in HTML               */
/* 2. target charset is searched for this unicode char, if it wasn't      */
/*  replaced. If not found, then 3. Substitution map is searched          */
/**************************************************************************/
const char *convert_char(int uc, char UTFbuffer[4]) {
	const char *mapped;

	if ((mapped=map_subst(spec_chars,uc))) return mapped;
    /* NULL target charset means UTF-8 output */
    return to_utf8(uc, UTFbuffer);
}
/******************************************************************/
/* Converts given unicode character to the utf-8 sequence         */
/* in the static string buffer. Buffer wouldbe overwritten upon   */
/* next call                                                      */
/******************************************************************/ 
char *to_utf8(unsigned int uc, char utfbuffer[4]) {
	int count=0;
	if (uc< 0x80) {
		utfbuffer[0]=uc;
		count=1;
	} else  {
		if (uc < 0x800) {
			utfbuffer[count++]=0xC0 | (uc >> 6);
		} else {
			utfbuffer[count++]=0xE0 | (uc >>12);
			utfbuffer[count++]=0x80 | ((uc >>6) &0x3F);
		}	    
		utfbuffer[count++]=0x80 | (uc & 0x3F);
	}  
	utfbuffer[count]=0;
	return utfbuffer;
}    

struct cp_map {
	int codepage;
    enum CHARSETS charset_name;
};

const struct cp_map cp_to_charset [] = {{437, CP_437},
                                        {850, CP_850},
                                        {852, CP_852},
                                        {855, CP_855},
                                        {857, CP_857},
                                        {860, CP_860},
                                        {861, CP_861},
                                        {862, CP_862},
                                        {863, CP_863},
                                        {864, CP_864},
                                        {865, CP_865},
                                        {866, CP_866},
                                        {869, CP_869},
                                        {874, CP_874},
                                        {1250, CP_1250},
                                        {1251, CP_1251},
                                        {1252, CP_1252},
                                        {1253, CP_1253},
                                        {1254, CP_1254},
                                        {1255, CP_1255},
                                        {1256, CP_1256},
                                        {1257, CP_1257},
                                        {1258, CP_1258},
                                        {10000, MAC_ROMAN},
                                        {10004, MAC_ARABIC},
                                        {10005, MAC_HEBREW},
                                        {10006, MAC_GREEK},
                                        {10007, MAC_CYRILLIC,},
                                        {10029, MAC_CENTEURO},
                                        {20866, KOI8_R},
                                        {28591, C8859_1},
                                        {28592, C8859_2},
                                        {28593, C8859_3},
                                        {28594, C8859_4},
                                        {28595, C8859_5},
                                        {28596, C8859_6},
                                        {28597, C8859_7},
                                        {28598, C8859_8},
                                        {28599, C8859_9},
                                        {28605, C8859_15},
                                        {0, C_NULL}};

enum CHARSETS charset_from_codepage(unsigned int codepage) {
	const struct cp_map *cp;

    for (cp = cp_to_charset;cp->codepage!=0&& cp->codepage!=codepage;cp++);
    return cp->charset_name;
}	
