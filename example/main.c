#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    char *buffer = NULL;

    char * src = NULL;
    size_t length;
    assert(argc == 2);
    FILE * f = fopen (argv[1], "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        src = malloc (length);
        if (src)
        {
            fread (src, 1, length, f);
        }
        fclose (f);
    }
    doc2text(src, length, &buffer);
    free(src);
    puts(buffer);
    free(buffer);

}
