#include <stdlib.h>
#include <string.h>
#include "port_linux.h"


int evq_init(ev_queue_t* evq, int size)
{
    evq->queue = malloc(sizeof(ev_data_t) * size);
    if (evq->queue) {
        memset(evq->queue, 0, sizeof(ev_data_t) * size);
        evq->size = size;
        return E_OK;
    }
    else {
        return E_NO_MEM;
    }
}

static int evq_is_full(ev_queue_t* evq)
{
    return (evq->write + 1) % evq->size == evq->read;
}

static int evq_is_empty(ev_queue_t* evq)
{
    return evq->read == evq->write;
}

int evq_put(ev_queue_t* evq, const port_event_t* event)
{
    int ret;
    if (evq_is_full(evq)) {
        ret = E_FULL;
    }
    else {
        evq->queue[evq->write].event = *event;
        evq->write = (evq->write + 1) % evq->size;
        ret = E_OK;
    }
    return ret;
}

int evq_get(ev_queue_t* evq, port_event_t* event)
{
    int ret;
    if (evq_is_empty(evq)) {
        ret = E_EMPTY;
    }
    else {
        *event = evq->queue[evq->read].event;
        evq->read = (evq->read + 1) % evq->size;
        ret = E_OK;
    }
    return ret;
}

int evq_close(ev_queue_t* evq)
{
    if (evq->queue) {
        free(evq->queue);
    }
    return E_OK;
}