#ifndef __CONNECTION_CPP__
#define __CONNECTION_CPP__
#include "Connectioon.h"

Connection::Connection(const char* ip, const char* port) {
    recvbuf = new char[1024 * 1024 + 1];
    memset(recvbuf, 0, 1024 * 1024);
    struct addrinfo* result = nullptr, * ptr = nullptr, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    int iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult) {
        printf("getaddrinfo failed %d\n", iResult);
        return;
    }
    sock = INVALID_SOCKET;
    ptr = result;
    sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (sock == INVALID_SOCKET) {
        printf("Error at socket() %ld\n", WSAGetLastError());
        return;
    }
    iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        return;
    }
    freeaddrinfo(result);
}

bool Connection::valid() {
    return sock != INVALID_SOCKET;
}

void Connection::write(const std::string& data) {
    int iResult = send(sock, data.c_str(), data.size(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed : %d\n", WSAGetLastError());
    }
    std::cout << "Sent : " << iResult << '\n';
}

std::string Connection::read() {
    memset(recvbuf, 0, 1024*1024);
    recv(sock, recvbuf, 1024 * 1024, 0);
    return std::string(recvbuf);
}

#endif