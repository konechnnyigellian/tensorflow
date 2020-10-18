

#ifndef TENSERFLOW_OCLGENERATOR_H
#define TENSERFLOW_OCLGENERATOR_H


namespace tenserflow {


class Algorithm;
class OclDevice;
class OclThreads;


using ocl_gen_config_fun = bool (*)(const OclDevice &device, const Algorithm &algorithm, OclThreads &threads);


} // namespace tenserflow


#endif /* TENSERFLOW_OCLGENERATOR_H */
