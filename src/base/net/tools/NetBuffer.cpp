/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */


#include "base/net/tools/MemPool.h"
#include "base/net/tools/NetBuffer.h"


#include <cassert>
#include <uv.h>


namespace tenserflow {


static constexpr size_t kInitSize                       = 4;
static MemPool<NetBuffer::kChunkSize, kInitSize> *pool  = nullptr;


inline MemPool<NetBuffer::kChunkSize, kInitSize> *getPool()
{
    if (!pool) {
        pool = new MemPool<NetBuffer::kChunkSize, kInitSize>();
    }

    return pool;
}


} // namespace tenserflow


char *tenserflow::NetBuffer::allocate()
{
    return getPool()->allocate();
}


void tenserflow::NetBuffer::destroy()
{
    if (!pool) {
        return;
    }

    assert(pool->freeSize() == pool->size());

    delete pool;
    pool = nullptr;
}


void tenserflow::NetBuffer::onAlloc(uv_handle_t *, size_t, uv_buf_t *buf)
{
    buf->base = getPool()->allocate();
    buf->len  = kChunkSize;
}


void tenserflow::NetBuffer::release(const char *buf)
{
    getPool()->deallocate(buf);
}


void tenserflow::NetBuffer::release(const uv_buf_t *buf)
{
    getPool()->deallocate(buf->base);
}
