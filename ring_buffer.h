#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t elem_size;
    size_t pos;
    void* buffer;
} RingBuffer;

void ring_buffer_init(RingBuffer* rb, size_t elem_size, size_t buf_size);
void ring_buffer_fill(RingBuffer* rb, void* data);
size_t ring_buffer_get_next(RingBuffer* rb);
void ring_buffer_free(RingBuffer* rb);

#endif 



#ifdef RING_BUFFER_IMPLEMENTATION

void ring_buffer_init(RingBuffer* rb, size_t elem_size, size_t buf_size) {
    assert(rb != NULL);
    assert(elem_size > 0);
    assert(buf_size > 0); 

    rb->size = buf_size;
    rb->elem_size = elem_size;
    rb->pos = buf_size - 1; // When get_next is used, will return index 0
    rb->buffer = malloc(elem_size * buf_size);
}

void ring_buffer_fill(RingBuffer* rb, void* data) {
    assert(rb != NULL);
    assert(data != NULL);

    uint8_t* start = (uint8_t*)(rb->buffer);
    uint8_t* end   = start + (rb->size * rb->elem_size);
    for (uint8_t* dest = start; dest < end; dest += rb->elem_size) {
        memcpy((void*)dest, data, rb->elem_size);
    }
}

size_t ring_buffer_get_next(RingBuffer* rb) {
    assert(rb != NULL);
    rb->pos = (rb->pos + 1) % rb->size;
    return rb->pos; 
}

void ring_buffer_free(RingBuffer* rb) {
    free(rb->buffer);
}

#endif
