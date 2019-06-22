
#include <malloc.h>

#include "RingBuffer.h"

RB_Status rb_init(ring_buffer_t **buffer, uint32_t size)
{
    *buffer = (ring_buffer_t *)malloc(sizeof(ring_buffer_t) + sizeof(int32_t) * size);
    if (*buffer) {
        (*buffer)->max = size;
        (*buffer)->head = 0;
        (*buffer)->tail = 0;
        (*buffer)->full = false;
        return RB_OK;
    } else
        return RB_ERROR;
}

RB_Status
rb_put(ring_buffer_t *buffer, int32_t item)
{
    if (buffer->full) {
        return RB_ERROR;
    }
    int32_t *entries = (uint32_t *)buffer + sizeof(ring_buffer_t);
    entries[buffer->tail++] = item;
    buffer->tail = buffer->tail % buffer->max;
    buffer->full = buffer->head == buffer->tail;
    return RB_OK;
}

extern RB_Status
rb_get(ring_buffer_t *buffer, int32_t *item)
{

    if ((buffer->head == buffer->tail) && !buffer->full) {
        return RB_ERROR;
    }
    int32_t *entries = (uint32_t *)buffer + sizeof(ring_buffer_t);
    *item = entries[buffer->head++];
    buffer->full = false;
    buffer->head = buffer->head % buffer->max;
    return RB_OK;
}
