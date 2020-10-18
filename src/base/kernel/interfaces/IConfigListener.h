

#ifndef TENSERFLOW_ICONFIGLISTENER_H
#define TENSERFLOW_ICONFIGLISTENER_H


namespace tenserflow {


class IConfig;


class IConfigListener
{
public:
    virtual ~IConfigListener() = default;

    virtual void onNewConfig(IConfig *config) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ICONFIGLISTENER_H
