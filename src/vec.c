/*
 * vec.c
 *
 *  Created on: Sep 30, 2017
 *      Author: Jeroen van der Heijden <jeroen@transceptor.technology>
 */
#include <string.h>
#include <stdlib.h>
#include <cleri/vec.h>

/*
 * Returns a new vec with size sz.
 */
cleri_vec_t * cleri_vec_new(uint32_t sz)
{
    cleri_vec_t * vec = (cleri_vec_t *) malloc(sizeof(cleri_vec_t) + sz * sizeof(void*));
    if (!vec) return NULL;
    vec->sz = sz;
    vec->n = 0;
    return vec;
}

/*
 * Destroy a vec with optional callback. If callback is NULL then it is
 * just as safe to simply call free() instead of this function.
 */
void cleri_vec_destroy(cleri_vec_t * vec, cleri_vec_destroy_cb cb)
{
    if (vec && cb) for (uint32_t i = 0; i < vec->n; i++)
    {
        (*cb)(cleri_vec_get(vec, i));
    }
    free(vec);
}

/*
 * Returns a copy of vec with an exact fit so the new vec->sz and vec->n will
 * be equal. In case of an allocation error the return value is NULL.
 */
cleri_vec_t * cleri_vec_dup(cleri_vec_t * vec)
{
    size_t sz = sizeof(cleri_vec_t) + vec->n * sizeof(void*);
    cleri_vec_t * v = (cleri_vec_t *) malloc(sz);
    if (!v) return NULL;
    memcpy(v, vec, sz);
    v->sz = v->n;
    return v;
}

/*
 * Append data to vec and returns vec.
 *
 * Returns a pointer to vec. The returned vec can be equal to the original
 * vec but there is no guarantee. The return value is NULL in case of an
 * allocation error.
 */
cleri_vec_t * cleri_vec_push(cleri_vec_t * vec, void * data)
{
    if (vec->n == vec->sz)
    {
        size_t sz = vec->sz;

        if (sz < 4)
        {
            vec->sz++;
        }
        else if (sz < 64)
        {
            vec->sz *= 2;
        }
        else
        {
            vec->sz += 64;
        }

        cleri_vec_t * tmp = (cleri_vec_t *) realloc(
                vec,
                sizeof(cleri_vec_t) + vec->sz * sizeof(void*));

        if (!tmp)
        {
            /* restore original size */
            vec->sz = sz;
            return NULL;
        }

        vec = tmp;
    }
    CLERI_VEC_push(vec, data);
    return vec;
}

/*
 * Extends a vec with n data elements and returns the new extended vec.
 *
 * In case of an error NULL is returned.
 */
cleri_vec_t * cleri_vec_extend(cleri_vec_t * vec, void * data[], uint32_t n)
{
    vec->n += n;
    if (vec->n > vec->sz)
    {
        cleri_vec_t * tmp = (cleri_vec_t *) realloc(
                vec,
                sizeof(cleri_vec_t) + vec->n * sizeof(void*));
        if (!tmp)
        {
            /* restore original length */
            vec->n -= n;
            return NULL;
        }

        vec = tmp;
        vec->sz = vec->n;
    }
    memcpy(vec->data + (vec->n - n), data, n * sizeof(void*));
    return vec;
}

/*
 * Resize a vec to sz. If the new size is smaller then vec->n might decrease.
 */
cleri_vec_t * cleri_vec_resize(cleri_vec_t * vec, uint32_t sz)
{
    cleri_vec_t * v = (cleri_vec_t *) realloc(
            vec,
            sizeof(cleri_vec_t) + sz * sizeof(void*));
    if (!v) return NULL;
    if (v->n > sz)
    {
        v->n = sz;
    }
    v->sz = sz;
    return v;
}

/*
 * Shrinks a vec to an exact fit.
 *
 * Returns a pointer to the new vec.
 */
cleri_vec_t * cleri_vec_shrink(cleri_vec_t * vec)
{
    if (vec->n == vec->sz) return vec;
    cleri_vec_t * v = (cleri_vec_t *) realloc(
            vec,
            sizeof(cleri_vec_t) + vec->n * sizeof(void*));
    if (!v) return NULL;
    v->sz = v->n;
    return v;
}


