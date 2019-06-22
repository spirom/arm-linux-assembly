

#include <stdint.h>
#include <stdbool.h>

#ifndef RPI_ASSEMBLY_RINGBUFFER_H
#define RPI_ASSEMBLY_RINGBUFFER_H

typedef enum {
    RB_OK, RB_ERROR
} RB_Status;

#define RB_SIZE 5

typedef struct {
    uint32_t head, tail, max;
    bool full;
} ring_buffer_t;

extern RB_Status
rb_init(ring_buffer_t **buffer, uint32_t size);

extern RB_Status
rb_put(ring_buffer_t *buffer, int32_t item);

extern RB_Status
rb_get(ring_buffer_t *buffer, int32_t *item);

#endif //RPI_ASSEMBLY_RINGBUFFER_H
