#ifndef NETWORK_LISTENER_H
#define NETWORK_LISTENER_H

#include <sys/types.h>
//#include <socket.h>

#include <string>

class NetworkListener
{
public:
    NetworkListener(const std::string& aInterface);
    ~NetworkListener();

    bool start();
    void stop();

private:
    bool setPromiscModeOn();
    bool setPromiscModeOff();

private:
    int m_socket;
    std::string m_interface;

    bool m_mustBeStopped;
};


#endif // NETWORK_LISTENER_H
