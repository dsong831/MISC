

#include <stdio.h>
#include <string.h>

#include "ringbuf.h"

#define MIN(a, b)               ((a) < (b) ? a : b)
#define WRAP(val, max)          ((val >= max) ? (val - max) : val)


#if defined(DEBUG_RINGBUF)
#define debug_ringbuf(fmt, args ...)   printf("RINGBUF: " fmt, ## args)
#else
#define debug_ringbuf(...)
#endif

void ringbuf_init(struct ringbuf *rbuf, uint32_t capacity, uint8_t *data_pool)
{
    memset(rbuf, 0, sizeof(struct ringbuf));
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->capacity = capacity;
    rbuf->data_pool = data_pool;
}

void ringbuf_reset(struct ringbuf *rbuf)
{
    rbuf->head = 0;
    rbuf->tail = 0;
}

uint8_t ringbuf_empty(struct ringbuf *rbuf)
{
    uint8_t empty;

    empty = (rbuf->head == rbuf->tail);

    return empty;
}

uint32_t ringbuf_space(struct ringbuf *rbuf)
{
    uint32_t space;

    if (rbuf->tail < rbuf->head)
    {
        space = (rbuf->capacity - rbuf->head) + rbuf->tail - 1;
    }
    else if (rbuf->tail == rbuf->head)
    {
        space = rbuf->capacity - 1;
    }
    else
    {
        space = rbuf->tail - rbuf->head - 1;
    }

    return space;
}

uint32_t ringbuf_filled(struct ringbuf *rbuf)
{
    uint32_t filled;

    filled = rbuf->capacity - ringbuf_space(rbuf) - 1;

    return filled;
}

uint8_t ringbuf_put(struct ringbuf *rbuf, uint8_t *data, uint32_t size)
{
    uint8_t *dest_data;
    uint32_t dest_size;
    uint32_t offset = 0;

    debug_ringbuf("put: start, size=%d\n", size);

    if (size > rbuf->capacity)
    {
        debug_ringbuf("put: overflow\n");
        return RINGBUF_SIZE_OVERFLOW;
    }

    if (size > ringbuf_space(rbuf))
    {
        debug_ringbuf("put: not enough\n");
        return RINGBUF_NOT_ENOUGH_SPACE;
    }

    if (rbuf->tail <= rbuf->head)
    {
        dest_data = &rbuf->data_pool[rbuf->head];
        dest_size = MIN(size, rbuf->capacity - rbuf->head);
        rbuf->head = WRAP(rbuf->head + dest_size, rbuf->capacity);
        memcpy(dest_data, data, dest_size);
        offset += dest_size;
        debug_ringbuf("put: part1, dest_size=%d, head=%d, tail=%d\n", dest_size, rbuf->head, rbuf->tail);
    }

    if (offset != size)
    {
        dest_data = &rbuf->data_pool[rbuf->head];
        dest_size = MIN(size - offset, rbuf->tail - rbuf->head - 1);
        rbuf->head += dest_size;
        memcpy(dest_data, data + offset, dest_size);
        debug_ringbuf("put: part2, dest_size=%d, head=%d, tail=%d\n", dest_size, rbuf->head, rbuf->tail);
    }

    debug_ringbuf("put: done, head=%d, tail=%d\n", rbuf->head, rbuf->tail);

    return RINGBUF_SUCCESS;
}

uint8_t ringbuf_get(struct ringbuf *rbuf, uint8_t *data, uint32_t size)
{
    uint8_t *src_data;
    uint32_t src_size;
    uint32_t offset = 0;

    debug_ringbuf("get: start, size=%d\n", size);

    if (size > rbuf->capacity)
    {
        debug_ringbuf("get: overflow\n");
        return RINGBUF_SIZE_OVERFLOW;
    }

    if (size > ringbuf_filled(rbuf))
    {
        debug_ringbuf("get: not enough\n");
        return RINGBUF_NOT_ENOUGH_DATA;
    }

    if (rbuf->tail > rbuf->head)
    {
        src_data = &rbuf->data_pool[rbuf->tail];
        src_size = MIN(size, rbuf->capacity - rbuf->tail);
        rbuf->tail = WRAP(rbuf->tail + src_size, rbuf->capacity);
        memcpy(data, src_data, src_size);
        offset += src_size;
        debug_ringbuf("get: part1, src_size=%d, head=%d, tail=%d\n", src_size, rbuf->head, rbuf->tail);
    }

    if (offset != size)
    {
        src_data = &rbuf->data_pool[rbuf->tail];
        src_size = MIN(size - offset, rbuf->head - rbuf->tail);
        rbuf->tail += src_size;
        memcpy(data + offset, src_data, src_size);
        debug_ringbuf("get: part2, src_size=%d, head=%d, tail=%d\n", src_size, rbuf->head, rbuf->tail);
    }

    debug_ringbuf("get: done, head=%d, tail=%d\n", rbuf->head, rbuf->tail);

    return RINGBUF_SUCCESS;
}

uint8_t ringbuf_get_claim(struct ringbuf *rbuf, uint8_t **data, uint32_t size)
{
    if (size > rbuf->capacity)
    {
        debug_ringbuf("clm: overflow\n");
        return RINGBUF_SIZE_OVERFLOW;
    }

    if (size > ringbuf_filled(rbuf))
    {
        debug_ringbuf("clm: not enough\n");
        return RINGBUF_NOT_ENOUGH_DATA;
    }

    debug_ringbuf("clm: %d\n", size);

    *data = &rbuf->data_pool[rbuf->tail];

    return RINGBUF_SUCCESS;
}

uint8_t ringbuf_get_release(struct ringbuf *rbuf, uint32_t size)
{
    uint32_t src_size;
    uint32_t offset = 0;

    if (size == 0)
    {
        return RINGBUF_SUCCESS;
    }

    if (rbuf->tail > rbuf->head)
    {
        src_size = MIN(size, rbuf->capacity - rbuf->tail);
        rbuf->tail = WRAP(rbuf->tail + src_size, rbuf->capacity);
        offset += src_size;
        debug_ringbuf("rel: part1, src_size=%d, head=%d, tail=%d\n", src_size, rbuf->head, rbuf->tail);
    }

    if (offset != size)
    {
        src_size = MIN(size - offset, rbuf->head - rbuf->tail);
        rbuf->tail += src_size;
        debug_ringbuf("rel: part2, src_size=%d, head=%d, tail=%d\n", src_size, rbuf->head, rbuf->tail);
    }

    debug_ringbuf("rel: done, head=%d, tail=%d\n", rbuf->head, rbuf->tail);

    return RINGBUF_SUCCESS;
}

