/*
 * queue.h
 *
 *  Created on: Oct 09, 2017
 *      Author: Jeroen van der Heijden <jeroen@transceptor.technology>
 */
#ifndef CLERI_QUEUE_H_
#define CLERI_QUEUE_H_

#define cleri_queue__i(q__, i__) ((q__)->s_ + i__) % (q__)->sz

typedef struct cleri_queue_s cleri_queue_t;
typedef void (*cleri_queue_destroy_cb)(void * data);

#include <stdio.h>

cleri_queue_t * cleri_queue_new(size_t sz);
void cleri_queue_destroy(cleri_queue_t * queue, cleri_queue_destroy_cb cb);
static inline void * cleri_queue_get(cleri_queue_t * queue, size_t i);
static inline void * cleri_queue_pop(cleri_queue_t * queue);
static inline void * cleri_queue_shift(cleri_queue_t * queue);
cleri_queue_t * cleri_queue_copy(cleri_queue_t * queue);
cleri_queue_t * cleri_queue_push(cleri_queue_t * queue, void * data);
cleri_queue_t * cleri_queue_unshift(cleri_queue_t * queue, void * data);
cleri_queue_t * cleri_queue_extend(
        cleri_queue_t * queue,
        void * data[],
        size_t n);
cleri_queue_t * cleri_queue_shrink(cleri_queue_t * queue);
/* unsafe macro for cleri_queue_push();
 * might overwrite data if not enough space */
#define CLERI_QUEUE_push(q__, d__) \
    (q__)->data_[cleri_queue__i(q__, (q__)->n++)] = d__
/* unsafe macro for cleri_queue_unshift();
 * might overwrite data if not enough space */
#define CLERI_QUEUE_unshift(q__, d__) \
        (q__)->data_[((q__)->s_ = (((q__)->s_ ? \
                (q__)->s_ : (q__)->sz) - (!!++(q__)->n)))] = d__;

struct cleri_queue_s
{
    size_t n;
    size_t sz;
    size_t s_;
    void * data_[];
};

static inline void * cleri_queue_get(cleri_queue_t * queue, size_t i)
{
    return queue->data_[cleri_queue__i(queue, i)];
}

static inline void * cleri_queue_pop(cleri_queue_t * queue)
{
    return (queue->n) ? queue->data_[cleri_queue__i(queue, --queue->n)] : NULL;
}

static inline void * cleri_queue_shift(cleri_queue_t * queue)
{
    return (queue->n && queue->n--) ?
            queue->data_[(((queue->s_ = cleri_queue__i(queue, 1))) ?
                    queue->s_: queue->sz ) - 1] : NULL;
}

#endif /* CLERI_QUEUE_H_ */
