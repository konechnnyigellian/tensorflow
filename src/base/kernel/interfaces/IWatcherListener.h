

#ifndef TENSERFLOW_IWATCHERLISTENER_H
#define TENSERFLOW_IWATCHERLISTENER_H


namespace tenserflow {


class String;


class IWatcherListener
{
public:
    virtual ~IWatcherListener() = default;

    virtual void onFileChanged(const String &fileName) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_IWATCHERLISTENER_H
