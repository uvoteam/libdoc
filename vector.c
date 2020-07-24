#include <stdlib.h>

#include "vector.h"


int vector_init(vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
    if (v->items == NULL) return -1;
    return 0;
}

static int vector_resize(vector *v, int capacity)
{
    void *items = realloc(v->items, sizeof(long int) * capacity);
    if (items != NULL) {
        v->items = items;
        v->capacity = capacity;
        return 0;
    } else {
        free(v->items);
        return -1;
    }
}

int vector_add(vector *v, long int item)
{
    if (v->capacity == v->total) {
        if (vector_resize(v, v->capacity * 2) == -1) return -1;
    }
    v->items[v->total++] = item;
    return 0;
}

void vector_free(vector *v)
{
    free(v->items);
}

void vector_reset(vector *v)
{
    v->total = 0;
}

int vector_find(vector *v, long int item)
{
    for (int i = 0; i < v->total; ++i) {
        if (v->items[i] == item) return 1;
    }
    return 0;
}
