/**
 * @file   ole.h
 * @author Alex Ott <ott@jet.msk.su>
 * @date   03 ��� 2003
 * Version: $Id: ole.h,v 1.1.1.1 2006/02/24 17:44:06 vitus Exp $
 * Copyright: Alex Ott, 2003, 
 * 
 * @brief  
 * 
 * 
 */

#ifndef _OLE_H
#define _OLE_H 1



#define BBD_BLOCK_SIZE     512
#define SBD_BLOCK_SIZE      64
#define PROP_BLOCK_SIZE    128
#define OLENAMELENGHT       32
#define MSAT_ORIG_SIZE     436

typedef enum {
	oleDir=1,
	oleStream=2,
	oleRootDir=5,
	oleUnknown=3
} oleType;

typedef struct {
	FILE *file;
	char name[OLENAMELENGHT+1];
	long int startBlock;
	long int curBlock;
	unsigned long int length;
	long int ole_offset;
	long int file_offset;
	unsigned char *dirPos;
	oleType type;
	long int numOfBlocks;
	long int *blocks;			/**< array of blocks numbers */
	int isBigBlock;
} oleEntry;

/**
 * Functions
 * 
 */
struct ole_params_t;
struct io_funcs_t;
FILE* ole_init(FILE *f, void *buffer, size_t bufSize, struct ole_params_t *ole_params);
FILE *ole_readdir(FILE *f, struct ole_params_t *ole_params);
int ole_seek(FILE *stream, long offset, int whence, struct ole_params_t *ole_params);
long ole_tell(FILE *stream);
int ole_eof(FILE *stream);
size_t ole_read(void *ptr, size_t size, size_t nmemb, FILE *stream, struct ole_params_t *ole_params);
int ole_open(FILE *);
int ole_close(FILE *);
void ole_finish(struct ole_params_t *ole_params);

extern size_t (*catdoc_read)(void *ptr, size_t size, size_t nmemb, FILE *stream, struct ole_params_t *ole_params);
extern int (*catdoc_eof)(FILE *stream);
extern int (*catdoc_seek)(FILE *stream, long offset, int whence, struct ole_params_t *ole_params);
extern long (*catdoc_tell)(FILE *stream);

void set_ole_func(struct io_funcs_t*);
void set_std_func(struct io_funcs_t*);

#endif /* _OLE_H */
