#ifndef LIBDOC_H
#define LIBDOC_H

#ifdef  __cplusplus
extern "C" {
#endif

extern int doc2text(const char *buf, size_t size, char **buffer_out);

#ifdef  __cplusplus
}
#endif

#endif //LIBDOC_H
