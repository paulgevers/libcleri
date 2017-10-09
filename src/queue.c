/*
 * queue.c
 *
 *  Created on: Oct 09, 2017
 *      Author: Jeroen van der Heijden <jeroen@transceptor.technology>
 */
#include <string.h>
#include <stdlib.h>
#include <cleri/queue.h>

static cleri_queue_t * cleri_queue__grow(cleri_queue_t * queue);

/*
 * Returns a new queue with size sz.
 */
cleri_queue_t * cleri_queue_new(size_t sz)
{
    cleri_queue_t * queue = (cleri_queue_t *) malloc(
            sizeof(cleri_queue_t) + sz * sizeof(void*));
    if (!queue) return NULL;
    queue->sz = sz;
    queue->n = 0;
    queue->s_ = 0;
    return queue;
}

/*
 * Destroy a queue with optional callback. If callback is NULL then it is
 * just as safe to simply call free() instead of this function.
 */
void cleri_queue_destroy(cleri_queue_t * queue, cleri_queue_destroy_cb cb)
{
    if (queue && cb) for (size_t i = 0; i < queue->n; i++)
    {
        (*cb)(cleri_queue_get(queue, i));
    }
    free(queue);
}

/*
 * Returns a copy of queue with an exact fit so the new queue->sz and queue->n
 * will be equal. In case of an allocation error the return value is NULL.
 */
cleri_queue_t * cleri_queue_copy(cleri_queue_t * queue)
{
    cleri_queue_t * q = (cleri_queue_t *) malloc(
            sizeof(cleri_queue_t) + queue->n * sizeof(void*));
    if (!q) return NULL;

    size_t m = queue->sz - queue->s_;
    if (m < queue->n)
    {
        memcpy(q->data_, queue->data_ + queue->s_, m * sizeof(void*));
        memcpy(q->data_ + m, queue->data_, (queue->n - m) * sizeof(void*));
    }
    else
    {
        memcpy(q->data_, queue->data_ + queue->s_, queue->n * sizeof(void*));
    }

    q->sz = q->n = queue->n;
    q->s_ = 0;

    return q;
}

/*
 * Push data to the queue and returns an address to the new queue.
 *
 * The returned queue can be equal to the original queue but there is no
 * guarantee. The return value is NULL in case of an allocation error.
 */
cleri_queue_t * cleri_queue_push(cleri_queue_t * queue, void * data)
{
    if (queue->n == queue->sz)
    {
        cleri_queue_t * tmp = cleri_queue__grow(queue);
        if (!tmp) return NULL;
        queue = tmp;
    }
    CLERI_QUEUE_push(queue, data);
    return queue;
}

cleri_queue_t * cleri_queue_unshift(cleri_queue_t * queue, void * data)
{
    if (queue->n == queue->sz)
    {
        cleri_queue_t * tmp = cleri_queue__grow(queue);
        if (!tmp) return NULL;
        queue = tmp;
    }
    CLERI_QUEUE_unshift(queue, data);

    return queue;
}

cleri_queue_t * cleri_queue_extend(
    cleri_queue_t * queue,
    void * data[],
    size_t n)
{
    size_t nn = queue->n + n;
    if (nn > queue->sz)
    {
        size_t sz = queue->sz;
        queue->sz = (nn > (queue->sz *= 2)) ? nn : queue->sz;

        cleri_queue_t * q = (cleri_queue_t *) realloc(
                queue,
                sizeof(cleri_queue_t) + queue->sz * sizeof(void*));

        if (!q)
        {
            /* restore original size */
            queue->sz = sz;
            return NULL;
        }
        queue = q;

        /* until q->s_ is always initialized but we can more than required */
        memcpy(queue->data_ + sz, queue->data_, queue->s_ * sizeof(void*));
    }

    size_t m, next = cleri_queue__i(queue, queue->n);
    if (next < queue->s_ || (n <= (m = queue->sz - next)))
    {
        memcpy(queue->data_ + next, data, n * sizeof(void*));
    }
    else
    {
        memcpy(queue->data_ + next, data, m * sizeof(void*));
        memcpy(queue->data_, data + m, (n - m) * sizeof(void*));
    }

    queue->n = nn;
    return queue;
}

/*
 * Shrinks a queue to an exact fit.
 *
 * Returns a pointer to the new queue.
 */
cleri_queue_t * cleri_queue_shrink(cleri_queue_t * queue)
{
    if (queue->n == queue->sz) return queue;

    size_t n = queue->sz - queue->s_;
    n = (n <= queue->n) ? n : queue->n;

    memmove(queue->data_ + queue->n - n,
            queue->data_ + queue->s_,
            n * sizeof(void*));

    cleri_queue_t * q = (cleri_queue_t *) realloc(
            queue,
            sizeof(cleri_queue_t) + queue->n * sizeof(void*));
    if (!q) return NULL;

    q->sz = q->n;
    q->s_ = q->n - n;

    return q;
}

static cleri_queue_t * cleri_queue__grow(cleri_queue_t * queue)
{
    size_t sz = queue->sz;

    queue->sz *= 2;
    queue->sz += !queue->sz;

    cleri_queue_t * q = (cleri_queue_t *) realloc(
            queue,
            sizeof(cleri_queue_t) + queue->sz * sizeof(void*));

    if (!q)
    {
        /* restore original size */
        queue->sz = sz;
        return NULL;
    }

    memcpy(q->data_ + sz, q->data_, q->s_ * sizeof(void*));

    return q;
}

