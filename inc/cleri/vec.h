/*
 * vec.h
 *
 *  Created on: Sep 30, 2017
 *      Author: Jeroen van der Heijden <jeroen@transceptor.technology>
 */
#ifndef CLERI_VEC_H_
#define CLERI_VEC_H_

typedef struct cleri_vec_s  cleri_vec_t;
typedef void (*cleri_vec_destroy_cb)(void * data);

#include <stdio.h>
#include <inttypes.h>

cleri_vec_t * cleri_vec_new(uint32_t sz);
void cleri_vec_destroy(cleri_vec_t * vec, cleri_vec_destroy_cb cb);
static inline uint32_t cleri_vec_space(cleri_vec_t * vec);
static inline void * cleri_vec_get(cleri_vec_t * vec, uint32_t i);
static inline void * cleri_vec_pop(cleri_vec_t * vec);
cleri_vec_t * cleri_vec_dup(cleri_vec_t * vec);
cleri_vec_t * cleri_vec_push(cleri_vec_t * vec, void * data);
cleri_vec_t * cleri_vec_extend(cleri_vec_t * vec, void * data[], uint32_t n);
cleri_vec_t * cleri_vec_resize(cleri_vec_t * vec, uint32_t sz);
cleri_vec_t * cleri_vec_shrink(cleri_vec_t * vec);

/* unsafe macro for cleri_vec_push() which assumes the vector has enough space */
#define CLERI_VEC_push(cleri_vec__, data_) (cleri_vec__)->data[(cleri_vec__)->n++] = data_

struct cleri_vec_s
{
    uint32_t n;
    uint32_t sz;
    void * data[];
};

static inline uint32_t cleri_vec_space(cleri_vec_t * vec)
{
    return vec->sz - vec->n;
}

static inline void * cleri_vec_get(cleri_vec_t * vec, uint32_t i)
{
    return vec->data[i];
}

static inline void * cleri_vec_pop(cleri_vec_t * vec)
{
    return (vec->n) ? vec->data[--vec->n] : NULL;
}


#endif /* CLERI_VEC_H_ */
