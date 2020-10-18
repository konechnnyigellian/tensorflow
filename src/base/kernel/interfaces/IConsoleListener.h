

#ifndef TENSERFLOW_ICONSOLELISTENER_H
#define TENSERFLOW_ICONSOLELISTENER_H


namespace tenserflow {


class IConsoleListener
{
public:
    virtual ~IConsoleListener() = default;

    virtual void onConsoleCommand(char command) = 0;
};


} /* namespace tenserflow */


#endif // TENSERFLOW_ICONSOLELISTENER_H
