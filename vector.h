#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_INIT_CAPACITY 4

typedef struct vector {
    long int *items;
    int capacity;
    int total;
} vector;

int vector_init(vector *);
int vector_add(vector *, long int);
void vector_free(vector *);
void vector_reset(vector *);
int vector_find(vector *, long int);

#endif
