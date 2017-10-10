/*
 * olist.c - linked list for keeping cleri objects.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <stdlib.h>
#include <cleri/olist.h>


/*
 * Returns 0 if successful or -1 in case of an error.
 * The list remains unchanged in case of an error and the object reference
 * counter will be incremented when successful.
 */
// int cleri__olist_append(cleri_olist_t * olist, cleri_t * cl_object)
// {
    // if (cl_object == NULL)
    // {
    //     return -1;
    // }

    // if (olist->cl_obj == NULL)
    // {
    //     cleri_incref(cl_object);
    //     olist->cl_obj = cl_object;
    //     olist->next = NULL;
    //     return 0;
    // }

    // while (olist->next != NULL)
    // {
    //     olist = olist->next;
    // }

    // olist->next = (cleri_olist_t *) malloc(sizeof(cleri_olist_t));

    // if (olist->next == NULL)
    // {
    //     return -1;
    // }

    // cleri_incref(cl_object);
    // olist->next->cl_obj = cl_object;
    // olist->next->next = NULL;

    // return 0;
// }

/*
 * Destroy the olist and decrement the reference counter for each object in
 * the list. (NULL is allowed as olist and does nothing)
 */
void cleri__olist_free(cleri_olist_t * olist)
{
    if (!olist) return;
    for (uint32_t i = 0; i < olist->n; i++)
    {
        cleri_free(olist->cl_obj[i]);
    }
    free(olist);
}




