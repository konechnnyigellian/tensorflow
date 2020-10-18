

#ifndef TENSERFLOW_CONTROLLER_H
#define TENSERFLOW_CONTROLLER_H


#include "base/kernel/Base.h"
#include "base/tools/Object.h"


namespace tenserflow {


class Job;
class Tenserflower;
class Network;


class Controller : public Base
{
public:
    TENSERFLOW_DISABLE_COPY_MOVE_DEFAULT(Controller)

    Controller(Process *process);
    ~Controller() override;

    int init() override;
    void start() override;
    void stop() override;

    Tenserflower *tenserflower() const;
    Network *network() const;
    void execCommand(char command);

private:
    Tenserflower *m_tenserflower     = nullptr;
    Network *m_network = nullptr;
};


} // namespace tenserflow


#endif /* TENSERFLOW_CONTROLLER_H */
