#ifndef ETHERNET_II_HANDLER_H
#define ETHERNET_II_HANDLER_H

#include "Handler.h"

class EthernetIIHandler : public Handler
{
public:
    EthernetIIHandler();

    virtual bool handlerRequest(std::string& request);
    virtual void setSuccessor(Handler *successor);
    virtual Handler* getNextSuccessor();

    bool getDestinationAddress(const std::string& buffer, std::string& destAddress);
    bool getSourceAddress(const std::string& buffer, std::string& sourceAddress);
    bool getType(const std::string& buffer, std::string& type);
    bool getData(const std::string& frame, std::string& data);
    bool getCRC(const std::string& frame, std::string& crc);

private:
    bool checkFrameLength(const size_t bufferSize);

private:
    static const unsigned MAC_ADDRESS_SIZE = 6;
    static const unsigned ETHERNET_TYPE_SIZE = 2;

    static const unsigned MIN_DATA_SIZE = 46;
    static const unsigned MAX_DATA_SIZE = 1500;

    static const unsigned CRC_CHECKSUMM = 4;

    static const unsigned MIN_FRAME_SIZE = 64;
    static const unsigned MAX_FRAME_SIZE = 1518;

private:
    Handler* m_successor;
    Handler* m_nextSuccessor;
};

#endif // ETHERNET_II_HANDLER_H
