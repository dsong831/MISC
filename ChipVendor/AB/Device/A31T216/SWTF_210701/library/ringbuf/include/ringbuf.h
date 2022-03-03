
#ifndef _RINGBUF_H_
#define _RINGBUF_H_

#include <stdint.h>

struct ringbuf
{
    uint32_t    head;
    uint32_t    tail;
    uint32_t    capacity;
    uint8_t *   data_pool;
};

struct ringbuf_seg
{
    uint8_t *   data;
    uint8_t     size;
};

enum ringbuf_status
{
    RINGBUF_SUCCESS,
    RINGBUF_SIZE_OVERFLOW,
    RINGBUF_NOT_ENOUGH_SPACE,
    RINGBUF_NOT_ENOUGH_DATA,
};

void ringbuf_init(struct ringbuf *rbuf, uint32_t capacity, uint8_t *data_pool);
void ringbuf_reset(struct ringbuf *rbuf);
uint8_t ringbuf_empty(struct ringbuf *rbuf);
uint32_t ringbuf_space(struct ringbuf *rbuf);
uint8_t ringbuf_put(struct ringbuf *rbuf, uint8_t *data, uint32_t size);
uint8_t ringbuf_get(struct ringbuf *rbuf, uint8_t *data, uint32_t size);
uint8_t ringbuf_get_claim(struct ringbuf *rbuf, uint8_t **data, uint32_t size);
uint8_t ringbuf_get_release(struct ringbuf *rbuf, uint32_t size);

#endif /* _RINGBUF_H_ */

