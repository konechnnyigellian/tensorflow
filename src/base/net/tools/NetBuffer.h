/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_NETBUFFER_H
#define TENSERFLOW_NETBUFFER_H


struct uv_buf_t;
using uv_handle_t = struct uv_handle_s;


#include <cstddef>


namespace tenserflow {


class NetBuffer
{
public:
    static constexpr size_t kChunkSize = 16 * 1024;

    static char *allocate();
    static void destroy();
    static void onAlloc(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void release(const char *buf);
    static void release(const uv_buf_t *buf);
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_NETBUFFER_H */
