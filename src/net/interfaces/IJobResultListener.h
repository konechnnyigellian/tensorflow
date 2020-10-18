

#ifndef TENSERFLOW_IJOBRESULTLISTENER_H
#define TENSERFLOW_IJOBRESULTLISTENER_H


namespace tenserflow {


class Client;
class JobResult;


class IJobResultListener
{
public:
    virtual ~IJobResultListener() = default;

    virtual void onJobResult(const JobResult &result) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IJOBRESULTLISTENER_H
