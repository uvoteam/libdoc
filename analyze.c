/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "catdoc.h"

const char ole_sign[]={0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1,0};
const char rtf_sign[]="{\\rtf";
const char old_word_sign[]={0xdb,0xa5,0};
const char write_sign[]={0x31,0xBE,0};

/********************************************************************* 
 * Determines format of input file and calls parse_word_header or 
 * process_file if
 * it is word processor file or copy_out if it is plain text file
 * return not 0 when error
 ********************************************************************/ 
int analyze_format(FILE *f, FILE *out) {
	unsigned char buffer[129];
	long offset=0;
	FILE *new_file, *ole_file;
	int ret_code=69;
    get_unicode_char_t get_unicode_char = NULL;
    struct ole_params_t ole_params = {0, 0, 0, NULL, 0, 0, 0, NULL, NULL, 0, 0, 0, 0, NULL, 0};
    struct io_funcs_t io_funcs;

	set_std_func(&io_funcs);
	io_funcs.catdoc_read(buffer,4,1,f, NULL);
	buffer[4]=0;
	if (strncmp(buffer,write_sign,2)==0) {
		get_unicode_char=get_8bit_char;
		return process_file(f,LONG_MAX, out, get_unicode_char, NULL, &io_funcs);
	} else if (strncmp(buffer,rtf_sign,4)==0) {
		return parse_rtf(f,out);
	} else if (strncmp(buffer,old_word_sign,2)==0) {
	   fread(buffer+4,1,124,f);	
	   return parse_word_header(buffer,f,128,0, out, get_unicode_char, NULL, &io_funcs);
	}	
	fread(buffer+4,1,4,f);
	if (strncmp(buffer,ole_sign,8)==0) {
		if ((new_file=ole_init(f, buffer, 8, &ole_params)) != NULL) {
			set_ole_func(&io_funcs);
			while((ole_file=ole_readdir(new_file, &ole_params)) != NULL) {
				int res=ole_open(ole_file);
				if (res >= 0) {
					if (strcmp(((oleEntry*)ole_file)->name , "WordDocument") == 0) {
						offset=io_funcs.catdoc_read(buffer, 1, 128, ole_file, &ole_params);
						ret_code=parse_word_header(buffer,ole_file,-offset,offset, out, get_unicode_char, &ole_params, &io_funcs);
                        if (ret_code == -1)
                        {
                            ole_close(ole_file);
                            ole_finish(&ole_params);
                            return -1;
                        }
					}
				} 
				ole_close(ole_file);
			}
			set_std_func(&io_funcs);
			ole_finish(&ole_params);
		} else {
            errno = EBADFD;
            return -1;
		}	
	} else {
		set_std_func(&io_funcs);
		ret_code = copy_out(f,buffer, out, get_unicode_char, NULL, &io_funcs);
		return ret_code;
	}
	
	return ret_code;
}   
#define fDot 0x0001   
#define fGlsy 0x0002
#define fComplex 0x0004
#define fPictures 0x0008 
#define fEncrypted 0x100
#define fReadOnly 0x400
#define fReserved 0x800
#define fExtChar 0x1000

/*******************************************************************/
/* parses word file info block passed in buffer.  
 * Determines actual size of text stream and calls process_file  
 ********************************************************************/
int parse_word_header(unsigned char * buffer,FILE *f,int offset,long curpos,
                      FILE *out, get_unicode_char_t get_unicode_char, struct ole_params_t *ole_params, struct io_funcs_t *io_funcs) {
	int flags, ret_code=0;
	long textstart,textlen,i;
	char buf[2];
    	
	flags = getshort(buffer,10);

	if (flags & fExtChar) {
		if (!get_unicode_char) 
			get_unicode_char=get_word8_char;

	} else if (!get_unicode_char) 
		get_unicode_char=get_8bit_char;

	if (flags & fEncrypted) return 69;
	
	/* skipping to textstart and computing textend */
	textstart=getlong(buffer,24);
	textlen=getlong(buffer,28)-textstart;
	textstart+=offset;
	for (i=0;i<textstart;i++) {
		io_funcs->catdoc_read(buf, 1, 1, f, ole_params);
		if (io_funcs->catdoc_eof(f)) {
            errno = EBADFD;
            return -1;
		}
	}    
	return process_file(f,textlen, out, get_unicode_char, ole_params, io_funcs) || ret_code;
}   
