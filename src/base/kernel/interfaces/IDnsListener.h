

#ifndef TENSERFLOW_IDNSLISTENER_H
#define TENSERFLOW_IDNSLISTENER_H


namespace tenserflow {


class Dns;


class IDnsListener
{
public:
    virtual ~IDnsListener() = default;

    virtual void onResolved(const Dns &dns, int status) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IDNSLISTENER_H
