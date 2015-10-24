/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
#ifndef CATDOC_H
#define CATDOC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* There is some strange thing on aix */
#if (defined(_AIX)||defined(___AIX)) && !defined(__unix)
# define __unix 1
#endif

/* These include files are always available */
#include <stdio.h>
#include <ctype.h>

/* This is our own file */
#include "ole.h"



/* Buffers for 32-bit and more program */
#define PARAGRAPH_BUFFER 262144



/* Buffer for single line. Should be greater than wrap margin +
  longest substitution sequence */
#define LINE_BUF_SIZE 512
/*   Default value for wrap margin */

#define WRAP_MARGIN 72


//enum values is important, it is the key to source_charsets
enum CHARSETS { C_NULL = -1,
                CP_437 = 0,
                CP_850,
                CP_852,
                CP_855,
                CP_857,
                CP_860,
                CP_861,
                CP_862,
                CP_863,
                CP_864,
                CP_865,
                CP_866,
                CP_869,
                CP_874,
                CP_1250,
                CP_1251,
                CP_1252,
                CP_1253,
                CP_1254,
                CP_1255,
                CP_1256,
                CP_1257,
                CP_1258,
                MAC_ROMAN,
                MAC_ARABIC,
                MAC_HEBREW,
                MAC_GREEK,
                MAC_CYRILLIC,
                MAC_CENTEURO,
                KOI8_R,
                C8859_1,
                C8859_2,
                C8859_3,
                C8859_4,
                C8859_5,
                C8859_6,
                C8859_7,
                C8859_8,
                C8859_9,
                C8859_15};


/*
 * Public types variables and procedures which should be avalable
 * to all files in the program
 */

/* Structure to store UNICODE -> target charset mappings */
/* array of 256 pointers (which may be null) to arrays of 256 short ints
   which contain 8-bit character codes or -1 if no matching char */
typedef short int  ** CHARSET;

/* structure to store multicharacter substitution mapping */
/* Array of 256 pointers to arrays of 256 pointers to string */
/* configuration variables defined in catdoc.c */
typedef const char SUBSTMAP[256][256][4];

extern SUBSTMAP replacements;
extern SUBSTMAP spec_chars;
extern const short int source_charsets[][256];

typedef size_t (*read_t)(void *ptr, size_t size, size_t nmemb, FILE *stream, struct ole_params_t *ole_params);
typedef int (*eof_t)(FILE *stream);
typedef int (*seek_t)(FILE *stream, long offset, int whence, struct ole_params_t *ole_params);
typedef long (*tell_t)(FILE *stream);

struct io_funcs_t
{
    read_t catdoc_read;
    eof_t catdoc_eof;
    seek_t catdoc_seek;
    tell_t catdoc_tell;
};

struct ole_params_t
{
    long int sectorSize;
    long int shortSectorSize;
    long int bbdNumBlocks;
    unsigned char *BBD;
    long int sbdNumber;
    long int sbdStart;
    long int sbdLen;
    unsigned char *SBD;
    oleEntry *rootEntry;
    long propCurNumber;
    long propLen;
    long propNumber;
    long propStart;
    unsigned char *properties;
    long int fileLength;
};

extern const short int const *source_charset;
extern SUBSTMAP spec_chars;
                /* Defines unicode chars which should be
                replaced by strings before UNICODE->target chatset
                mappigs are applied i.e. TeX special chars like %
                */
extern SUBSTMAP replacements;
                 /* Defines unicode chars which could be
                    mapped to some character sequence if no
                    corresponding character exists in the target charset
                    i.e copyright sign */

typedef int (*get_unicode_char_t)(FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                                  unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs);
/* pointer to function which gets a char from stream */

extern int get_utf16lsb (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                         unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs);
extern int get_utf16msb (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                         unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs);
extern int get_utf8 (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params, unsigned char read_buf[256],
                     int *buf_is_unicode, struct io_funcs_t *io_funcs);
extern int get_8bit_char (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                          unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs);

extern int get_word8_char (FILE *f,long *offset,long fileend, struct ole_params_t *ole_params,
                           unsigned char read_buf[256], int *buf_is_unicode, struct io_funcs_t *io_funcs);

extern enum CHARSETS charset_from_codepage(unsigned int codepage);
extern  short int *read_charset(const char *filename);


extern int to_unicode (const short int *charset, int c) ;

extern int from_unicode (CHARSET charset, int u) ;

extern const char* convert_char(int unicode_char);

extern char* to_utf8(unsigned int uc, char UTFbuffer[4]);


#ifdef HAVE_LANGINFO
void get_locale_charset(void);
#if  defined(HAVE_STRFTIME) && !defined(__TURB0C__)
void	set_time_locale();
#endif	
#endif

#define LONGEST_SEQUENCE 6 /* for checking which value of wrap_margin
                             can cause buffer overflow*/

const char *map_subst(SUBSTMAP map,int uc);

int check_charset(char **filename,const char *charset);
int process_file(FILE *f,long stop, FILE *out, get_unicode_char_t get_unicode_char, struct ole_params_t *ole_params, struct io_funcs_t *io_funcs);
int copy_out(FILE *f, char *header, FILE *out, get_unicode_char_t get_unicode_char, struct ole_params_t *ole_params, struct io_funcs_t *io_funcs);
void output_paragraph(unsigned short int *buffer, FILE *out, char outputbuffer[LINE_BUF_SIZE], int *bufpos);
int parse_rtf(FILE *f, FILE *out);
/* format recognition*/
int analyze_format(FILE *f, FILE *out);
void list_charsets(void);
int parse_word_header(unsigned char *buffer,FILE *f,int offset,long curpos,
                      FILE *out, get_unicode_char_t get_unicode_char, struct ole_params_t *ole_params, struct io_funcs_t *io_funcs);
/* large buffers for file IO*/

/* numeric conversions */	
long int getlong(unsigned char *buffer,int offset);
unsigned long int getulong(unsigned char *buffer,int offset);
unsigned int getshort(unsigned char *buffer,int offset);
#endif
