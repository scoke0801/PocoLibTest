//
// Created by gusl0 on 2021-08-05.
//

#ifndef POCOMOBILESAMPLE_NETTEST_H
#define POCOMOBILESAMPLE_NETTEST_H

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"

constexpr Poco::UInt16 PORT_NUM = 3500;

constexpr char IP_ADDR[] = "211.38.43.53";

constexpr int MAX_SEND_COUNT = 5;

class RNGClient
{
public:
    RNGClient();
    ~RNGClient();
    void Run();

    void Send(Poco::Net::StreamSocket& socket, std::string text);
    void* Receive(Poco::Net::StreamSocket& socket);

private:
    Poco::Net::StreamSocket m_Socket;
};

#endif //POCOMOBILESAMPLE_NETTEST_H
