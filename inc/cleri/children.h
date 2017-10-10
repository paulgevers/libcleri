/*
 * children.h - linked list for keeping node results
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *  - refactoring, 17-06-2017
 */
#ifndef CLERI_CHILDREN_H_
#define CLERI_CHILDREN_H_

#include <cleri/node.h>

/* typedefs */
typedef struct cleri_node_s cleri_node_t;
typedef struct cleri_children_s cleri_children_t;

/* private functions */
void cleri__children_free(cleri_children_t * children);
// int cleri__children_add(cleri_children_t * children, cleri_node_t * node);

/* structs */
struct cleri_children_s
{
    uint32_t n;
    uint32_t sz_;
    cleri_node_t * node[];
};

#endif /* CLERI_CHILDREN_H_ */