#ifndef __CONECTION_H__
#define __CONECTION_H__
#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")


class  Connection {
public:
    Connection(const char* ip, const char* port);
    std::string read();
    void write(const std::string&);
    bool valid();
    ~Connection() {
        delete[]recvbuf;
    }
protected:
    SOCKET sock;
    char* recvbuf;
};

#endif