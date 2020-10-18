


#include "backend/opencl/wrappers/OclContext.h"
#include "backend/opencl/runners/tools/OclSharedState.h"
#include "backend/opencl/wrappers/OclLib.h"


tenserflow::OclContext::OclContext(const OclDevice &device)
{
    std::vector<cl_device_id> ids = { device.id() };
    m_ctx = OclLib::createContext(ids);
}


tenserflow::OclContext::~OclContext()
{
    if (m_ctx) {
        OclLib::release(m_ctx);
    }
}


bool tenserflow::OclContext::init(const std::vector<OclDevice> &devices, std::vector<OclLaunchData> &threads)
{
    if (!m_ctx) {
        std::vector<cl_device_id> ids(devices.size());
        for (size_t i = 0; i < devices.size(); ++i) {
            ids[i] = devices[i].id();
        }

        m_ctx = OclLib::createContext(ids);
    }

    if (!m_ctx) {
        return false;
    }

    for (OclLaunchData &data : threads) {
        data.ctx = m_ctx;
    }

    return true;
}
