#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

class WebSocketServer {
public:
    WebSocketServer() {
        // Initialize Winsock
        int iResult = WSAStartup(MAKEWORD(2,2), &this->wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed: %d\n", iResult);
        }
    }

    ~WebSocketServer() {
        WSACleanup();
    }

    void init(uint16_t port) {

    }

private:
    WSADATA wsaData;

};