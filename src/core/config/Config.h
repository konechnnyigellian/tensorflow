

#ifndef TENSERFLOW_CONFIG_H
#define TENSERFLOW_CONFIG_H


#include <cstdint>


#include "3rdparty/rapidjson/fwd.h"
#include "backend/cpu/CpuConfig.h"
#include "base/kernel/config/BaseConfig.h"
#include "base/tools/Object.h"


namespace tenserflow {


class ConfigPrivate;
class CudaConfig;
class IThread;
class OclConfig;
class RxConfig;


class Config : public BaseConfig
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE(Config);

    Config();
    ~Config() override;

    const CpuConfig &cpu() const;

#   ifdef TENSERFLOW_FEATURE_OPENCL
    const OclConfig &cl() const;
#   endif

#   ifdef TENSERFLOW_FEATURE_CUDA
    const CudaConfig &cuda() const;
#   endif

#   ifdef TENSERFLOW_ALGO_RANDOMX
    const RxConfig &rx() const;
#   endif

#   if defined(TENSERFLOW_FEATURE_NVML) || defined (TENSERFLOW_FEATURE_ADL)
    uint32_t healthPrintTime() const;
#   else
    uint32_t healthPrintTime() const { return 0; }
#   endif

    bool isShouldSave() const;
    bool read(const IJsonReader &reader, const char *fileName) override;
    void getJSON(rapidjson::Document &doc) const override;

private:
    ConfigPrivate *d_ptr;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CONFIG_H */
