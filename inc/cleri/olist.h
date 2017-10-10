/*
 * olist.h - linked list for keeping cleri objects.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_OLIST_H_
#define CLERI_OLIST_H_

/* typedefs */
typedef struct cleri_olist_s cleri_olist_t;
typedef struct cleri_s cleri_t;

#include <inttypes.h>
#include <cleri/cleri.h>

void cleri__olist_free(cleri_olist_t * olist);

struct cleri_olist_s
{
    uint32_t n;
    uint32_t sz_;
    cleri_t * cl_obj[];
};

#endif /* CLERI_OLIST_H_ */