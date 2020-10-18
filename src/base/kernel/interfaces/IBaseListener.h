

#ifndef TENSERFLOW_IBASELISTENER_H
#define TENSERFLOW_IBASELISTENER_H


namespace tenserflow {


class Config;


class IBaseListener
{
public:
    virtual ~IBaseListener() = default;

    virtual void onConfigChanged(Config *config, Config *previousConfig) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IBASELISTENER_H
