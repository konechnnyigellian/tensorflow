/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_ADLLIB_H
#define TENSERFLOW_ADLLIB_H


#include "backend/opencl/wrappers/AdlHealth.h"


namespace tenserflow {


class OclDevice;


class AdlLib
{
public:
    static bool init();
    static const char *lastError() noexcept;
    static void close();

    static AdlHealth health(const OclDevice &device);

    static inline bool isInitialized() noexcept         { return m_initialized; }
    static inline bool isReady() noexcept               { return m_ready; }

private:
    static bool dlopen();
    static bool load();

    static bool m_initialized;
    static bool m_ready;
};


} // namespace tenserflow


#endif /* TENSERFLOW_ADLLIB_H */
