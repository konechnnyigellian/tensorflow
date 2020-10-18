


#include "core/Controller.h"
#include "backend/cpu/Cpu.h"
#include "core/config/Config.h"
#include "core/Tenserflower.h"
#include "crypto/common/VirtualMemory.h"
#include "net/Network.h"


#include <cassert>


tenserflow::Controller::Controller(Process *process) :
    Base(process)
{
}


tenserflow::Controller::~Controller()
{
    delete m_network;

    VirtualMemory::destroy();
}


int tenserflow::Controller::init()
{
    Base::init();

    VirtualMemory::init(config()->cpu().memPoolSize(), config()->cpu().isHugePages());

    m_network = new Network(this);

    return 0;
}


void tenserflow::Controller::start()
{
    Base::start();

    m_tenserflower = new Tenserflower(this);

    network()->connect();
}


void tenserflow::Controller::stop()
{
    Base::stop();

    delete m_network;
    m_network = nullptr;

    m_tenserflower->stop();

    delete m_tenserflower;
    m_tenserflower = nullptr;
}


tenserflow::Tenserflower *tenserflow::Controller::tenserflower() const
{
    assert(m_tenserflower != nullptr);

    return m_tenserflower;
}


tenserflow::Network *tenserflow::Controller::network() const
{
    assert(m_network != nullptr);

    return m_network;
}


void tenserflow::Controller::execCommand(char command)
{
    tenserflower()->execCommand(command);
    network()->execCommand(command);
}
