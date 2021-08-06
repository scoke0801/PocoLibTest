//
// Created by gusl0 on 2021-08-05.
//

#include "NetTest.h"

RNGClient::RNGClient()
{
    m_Socket.connect(Poco::Net::SocketAddress(IP_ADDR, PORT_NUM));
}

RNGClient::~RNGClient()
{
    m_Socket.close();
}

void RNGClient::Send(Poco::Net::StreamSocket &socket, std::string text)
{
    Poco::Timespan timeout(1);

    while (socket.poll(timeout, Poco::Net::Socket::SELECT_WRITE) == false) {
        // wait
    }

    if (true != text.empty()) {
        int len = sizeof(text);
        socket.sendBytes(text.c_str(), len);
    }
}

void* RNGClient::Receive(Poco::Net::StreamSocket& socket)
{
    Poco::Timespan timeout(1);

    // 서버로부터 메시지를 받을 때까지 대기 중..
    while (socket.poll(timeout, Poco::Net::Socket::SELECT_READ) == false) {
        // wait
    }
    int rng;
    int len = socket.receiveBytes((void*)&rng, sizeof(rng));
    return nullptr;
}
