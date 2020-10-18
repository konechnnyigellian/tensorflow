

#ifndef TENSERFLOW_CPU_H
#define TENSERFLOW_CPU_H


#include "backend/cpu/interfaces/ICpuInfo.h"


namespace tenserflow {


class Cpu
{
public:
    static ICpuInfo *info();
    static rapidjson::Value toJSON(rapidjson::Document &doc);
    static void release();

    inline static Assembly::Id assembly(Assembly::Id hint) { return hint == Assembly::AUTO ? Cpu::info()->assembly() : hint; }
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_CPU_H */
