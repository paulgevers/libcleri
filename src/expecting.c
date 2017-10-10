/*
 * expecting.c - holds elements which the grammar expects at one position.
 *               this can be used for suggestions.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <cleri/expecting.h>
#include <stdlib.h>
#include <assert.h>
#include <cleri/vec.h>

static cleri_exp_modes_t * EXPECTING_modes_new(const char * str, int mode);
static void EXPECTING_empty(cleri_expecting_t * expecting);
static int EXPECTING_get_mode(cleri_queue_t * modes, const char * str);
static void EXPECTING_shift_modes(cleri_queue_t * modes, const char * str);

/*
 * Returns NULL in case an error has occurred.
 */
cleri_expecting_t * cleri__expecting_new(const char * str)
{
    cleri_expecting_t * expecting =
            (cleri_expecting_t *) malloc(sizeof(cleri_expecting_t));

    if (expecting != NULL)
    {
        cleri_exp_modes_t * exptmode;
        expecting->str = str;

        if ((expecting->required = (cleri_olist_t *) cleri_vec_new(8)) == NULL)
        {
            free(expecting);
            return NULL;
        }

        if ((expecting->optional = (cleri_olist_t *) cleri_vec_new(8)) == NULL)
        {
            free(expecting->required);
            free(expecting);
            return NULL;
        }

        if ((expecting->modes = cleri_queue_new(8)) == NULL)
        {
            free(expecting->optional);
            free(expecting->required);
            free(expecting);
            return NULL;
        }
        exptmode = EXPECTING_modes_new(str, CLERI__EXP_MODE_REQUIRED);

        if (exptmode == NULL)
        {
            free(expecting->optional);
            free(expecting->required);
            free(expecting->modes);
            free(expecting);
            return NULL;
        }
        CLERI_QUEUE_push(expecting->modes, exptmode);
    }

    return expecting;
}

/*
 * Returns 0 if the mode is set successful and -1 if an error has occurred.
 */
int cleri__expecting_update(
        cleri_expecting_t * expecting,
        cleri_t * cl_obj,
        const char * str)
{
    if (str > expecting->str)
    {
        EXPECTING_empty(expecting);
        expecting->str = str;
        EXPECTING_shift_modes(expecting->modes, str);
    }

    if (expecting->str == str)
    {
        cleri_vec_t * tmp;
        if (EXPECTING_get_mode(expecting->modes, str))
        {
            /* true (1) is required */
            tmp = cleri_vec_push((cleri_vec_t *) expecting->required, cl_obj);
            if (!tmp) return -1;
            expecting->required = (cleri_olist_t *) tmp;
        }
        else
        {
            /* false (0) is optional */
            tmp = cleri_vec_push((cleri_vec_t *) expecting->optional, cl_obj);
            if (!tmp) return -1;
            expecting->optional = (cleri_olist_t *) tmp;
        }
    }

    return 0;
}

/*
 * Returns 0 if the mode is set successful and -1 if an error has occurred.
 */
int cleri__expecting_set_mode(
        cleri_expecting_t * expecting,
        const char * str,
        int mode)
{
    cleri_queue_t * tmp;
    cleri_exp_modes_t * m;
    for (size_t i = 0; i < expecting->modes->n; i++)
    {
        m = (cleri_exp_modes_t *) cleri_queue_get(expecting->modes, i);
        if (m->str == str)
        {
            m->mode = mode && m->mode;
            return 0;
        }
    }

    m = EXPECTING_modes_new(str, mode);
    if (!m) return -1;

    tmp = cleri_queue_push(expecting->modes, m);
    if (!tmp)
    {
        free(m);
        return -1;
    }
    expecting->modes = tmp;
    return 0;

    // cleri_exp_modes_t * current = expecting->modes;
    // for (; current->next != NULL; current = current->next)
    // {
    //     if (current->str == str)
    //     {
    //         current->mode = mode && current->mode;
    //         return 0;
    //     }
    // }
    // current->next = (cleri_exp_modes_t *) malloc(sizeof(cleri_exp_modes_t));

    // if (current->next == NULL)
    // {
    //     return -1;
    // }

    // current->next->mode = mode;
    // current->next->next = NULL;
    // current->next->str = str;

    // return 0;
}

/*
 * Destroy expecting object.
 */
void cleri__expecting_free(cleri_expecting_t * expecting)
{
    if (!expecting) return;
    free(expecting->required);
    free(expecting->optional);
    cleri_queue_destroy(expecting->modes, free);
    free(expecting);
}

/*
 * append optional to required and sets optional to NULL
 */
int cleri__expecting_combine(cleri_expecting_t * expecting)
{
    if (cleri_vec_space(
            (cleri_vec_t *) expecting->required) < expecting->optional->n)
    {
        cleri_vec_t * tmp = cleri_vec_resize(
            (cleri_vec_t *) expecting->required,
            expecting->required->n + expecting->optional->n);
        if (!tmp) return -1;
        expecting->required = (cleri_olist_t *) tmp;
    }

    for (size_t i = 0; i < expecting->optional->n; i++)
    {
        CLERI_VEC_push(
            (cleri_vec_t *) expecting->required,
            cleri_vec_get((cleri_vec_t *) expecting->optional, i));
    }


    /* cleanup optional */
    free(expecting->optional);
    expecting->optional = NULL;

    return 0;

    // cleri_olist_t * required = expecting->required;

    // if (expecting->optional->cl_obj == NULL)
    // {
    //     free(expecting->optional);
    //     expecting->optional = NULL;
    // }

    // if (required->cl_obj == NULL)
    // {
    //     free(expecting->required);
    //     expecting->required = expecting->optional;
    // }
    // else
    // {
    //     while (required->next != NULL)
    //     {
    //         required = required->next;
    //     }
    //     required->next = expecting->optional;
    // }
    // expecting->optional = NULL;
}

/*
 * Returns NULL in case an error has occurred.
 */
static cleri_exp_modes_t * EXPECTING_modes_new(const char * str, int mode)
{
    cleri_exp_modes_t * modes =
            (cleri_exp_modes_t *) malloc(sizeof(cleri_exp_modes_t));
    if (modes != NULL)
    {
        modes->mode = mode;
        modes->str = str;
    }
    return modes;
}

/*
 * shift from modes
 */
static void EXPECTING_shift_modes(cleri_queue_t * modes, const char * str)
{

    cleri_exp_modes_t * mode;
    while (modes->n)
    {
        mode = (cleri_exp_modes_t *) cleri_queue_get(modes, 0);
        if (mode->str == str) break;
        cleri_queue_shift(modes);
        free(mode);
    }

    // while ((*modes)->next != NULL)
    // {
    //     if ((*modes)->str == str)
    //     {
    //         break;
    //     }
    //     next = (*modes)->next;
    //     free(*modes);
    //     *modes = next;
    // }
}


/*
 * Return modes for a given position in str.
 */
static int EXPECTING_get_mode(cleri_queue_t * modes, const char * str)
{
    cleri_exp_modes_t * mode;
    for (size_t i = 0; i < modes->n; i++)
    {
        mode = (cleri_exp_modes_t *) cleri_queue_get(modes, i);
        if (mode->str == str) return mode->mode;
    }
    return CLERI__EXP_MODE_REQUIRED;


    // for (; modes != NULL; modes = modes->next)
    // {
    //     if (modes->str == str)
    //     {
    //         return modes->mode;
    //     }
    // }
    // return CLERI__EXP_MODE_REQUIRED;
}

/*
 * Empty both required and optional lists.
 */
static void EXPECTING_empty(cleri_expecting_t * expecting)
{
    expecting->required->n = 0;
    expecting->optional->n = 0;
}
