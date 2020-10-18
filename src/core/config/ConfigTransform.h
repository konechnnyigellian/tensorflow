

#ifndef TENSERFLOW_CONFIGTRANSFORM_H
#define TENSERFLOW_CONFIGTRANSFORM_H


#include "base/kernel/config/BaseTransform.h"


namespace tenserflow {


class ConfigTransform : public BaseTransform
{
protected:
    void finalize(rapidjson::Document &doc) override;
    void transform(rapidjson::Document &doc, int key, const char *arg) override;

private:
    void transformBoolean(rapidjson::Document &doc, int key, bool enable);
    void transformUint64(rapidjson::Document &doc, int key, uint64_t arg);

    bool m_opencl           = false;
    int64_t m_affinity      = -1;
    uint64_t m_intensity    = 1;
    uint64_t m_threads      = 0;
};


} // namespace tenserflow


#endif /* TENSERFLOW_CONFIGTRANSFORM_H */
