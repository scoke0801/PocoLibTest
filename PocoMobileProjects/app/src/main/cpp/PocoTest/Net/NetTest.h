//
// Created by gusl0 on 2021-08-05.
//

#ifndef POCOMOBILESAMPLE_NETTEST_H
#define POCOMOBILESAMPLE_NETTEST_H

#include "Poco/Net/SocketStream.h"

#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/CipherFactory.h"

class NetTest {
    NetTest(){

        Poco::Crypto::Cipher* pCipher;
        Poco::Crypto::CipherFactory &factory = Poco::Crypto::CipherFactory::defaultFactory();
        Poco::Crypto::CipherKey key("aes256");
        pCipher = factory.createCipher(key);
    }
};


#endif //POCOMOBILESAMPLE_NETTEST_H
