#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <atomic>

#include "DataStructures.h"

#pragma comment(lib, "Ws2_32.lib")

class WebSocketServer {
public:
    WebSocketServer();

    ~WebSocketServer();

    bool initialize(int port);

    // Start the server
    void start();

    // Stop the server and clean
    void stop();

private:
    // Worker loop for handling client connections and messages
    void workerLoop();

    // Debug loop for printing metrics
    void debugLoop();
    
private:
    // Winsock data structure
    WSADATA wsa;
    // Socket descriptors
    SOCKET s, new_socket;
    // Server and client address structures
    struct sockaddr_in server, client;
    // Size of the client address structure
    int c;
    // Buffer for incoming messages (not used yet)
    char *message;
    // Flag to control the worker loop
    std::atomic<bool> isRunning;
    // Thread for the worker loop
    std::thread workerThread;
    // Thread for the debug loop
    std::thread debugThread;
    // Metrics for tracking network performance
    NetworkMetrics metrics;
};