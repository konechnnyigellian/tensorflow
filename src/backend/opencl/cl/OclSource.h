

#ifndef TENSERFLOW_OCLSOURCE_H
#define TENSERFLOW_OCLSOURCE_H


namespace tenserflow {


class Algorithm;


class OclSource
{
public:
    static const char *get(const Algorithm &algorithm);
    static void init();
};


} // namespace tenserflow


#endif /* TENSERFLOW_OCLSOURCE_H */
