/*****************************************************************/
/* Output of unicode buffers with conversion into target encoding*/
/* And application of substitution maps                          */
/*                                                               */
/* This file is part of catdoc project                           */
/* (c) Victor Wagner 1996-2003, (c) Alex Ott 2003	             */
/*****************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "catdoc.h"
/************************************************************************/
/* performs paragraph formatting if wrap_margin is non-zero             */
/* gets character sequence and appends it to buffer. If buffer is long  */
/* enough, prints its beginning out                                     */
/* Input parameter is pointer to the string which represents one unicode*/
/* character after character set translation. See convert_char ina      */
/* charset.c                                                            */
/************************************************************************/

void out_char(const char *chunk, FILE *out, char outputbuffer[LINE_BUF_SIZE], int *bufpos) {
	int eol_flag=0;
	const char *p; char *q;
	if (!WRAP_MARGIN) {
		fputs(chunk, out);
		return;
	}

	for (q=outputbuffer+(*bufpos),p=chunk;
         *p;
         *(q++)=*(p++),(*bufpos)++) {
			if (*p=='\n') eol_flag=1;
	}		
	*q=0;
		 /* This strcat is safe. wrap margin setting
							 code in main.c ensures that wrap_margin is 
							 less than LINE_BUF_SIZE-strlen(largest chunk)
							 */  
	if (eol_flag) {
		/* End of paragraph */
		const char *q = map_subst(spec_chars,'\n');
		fputs(outputbuffer, out);
		*outputbuffer=0;
		*bufpos=0;
		if (q) {
			 fputs(q,out);
		} else {
			fputc('\n',out);
		}	
	} else if (*bufpos>WRAP_MARGIN) {
		char *q=outputbuffer,*p=outputbuffer+WRAP_MARGIN;
		
		while (p>outputbuffer&&!isspace(*p)) p--;
		if (p==outputbuffer) {
			/*worst case - nowhere to wrap. Will use brute force */
			fwrite(outputbuffer,WRAP_MARGIN,1,out);
			fputc('\n',out);
			p=outputbuffer+WRAP_MARGIN;
		} else {
			*p=0;p++;
			fputs(outputbuffer,out);
			fputc('\n',out);
		}
		for(q=outputbuffer;*p;p++,q++) *q=*p;
		*bufpos=q-outputbuffer;
		*q=0;
	}
}

/************************************************************************/
/* Main output function.
 * Programs which read word-processor files should accumulate paragraphs
 * in the buffer as array of unicode 16-bit codes and pass to this
 * function
 ************************************************************************/ 
void output_paragraph(unsigned short int *buffer, FILE *out, char outputbuffer[LINE_BUF_SIZE], int *bufpos) {
	unsigned short int *p;
	int countout=0;

	for (p=buffer;*p;p++) {
		out_char(convert_char(*p), out, outputbuffer, bufpos);
		countout++;
	}
}
