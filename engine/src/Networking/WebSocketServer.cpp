#include "WebSocketServer.h"

WebSocketServer::WebSocketServer() { };

WebSocketServer::~WebSocketServer() { };

bool WebSocketServer::initialize(int port) {
    std::cout << "[DEBUG] Initializing Winsock..." << std::endl;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return false;

    // Create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) return false;

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Bind and listen
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) return false;
    if (listen(s, 3) == SOCKET_ERROR) return false;

    // Success
    return true;
}

void WebSocketServer::start() {
    if (!isRunning) {
        isRunning = true;
        workerThread = std::thread(&WebSocketServer::workerLoop, this);
        debugThread = std::thread(&WebSocketServer::debugLoop, this);
    }
}

void WebSocketServer::stop() {
    isRunning = false;
    // Closing the listen socket forces accept() 
    // to unblock and return an error
    if (s != INVALID_SOCKET) {
        closesocket(s);
    }
    
    // Wait for the background thread to finish its work before destroying
    if (workerThread.joinable()) {
        workerThread.join();
    }
    
    WSACleanup();
}

void WebSocketServer::workerLoop() {
    // c is the size of the client address structure
    c = sizeof(struct sockaddr_in);

    // clientSocket holds socket descriptor for accepted client connection
    SOCKET clientSocket;

    while (isRunning) {
        // accept() blocks the thread until a client connects
        clientSocket = accept(s, (struct sockaddr *)&client, &c);

        
        if (clientSocket == INVALID_SOCKET) {
            if (!isRunning) break; // Expected behavior during shutdown
            std::cout << "[ERROR] Accept failed. Code: " << WSAGetLastError() << std::endl;
            continue; 
        }

        // Set the client socket to non-blocking mode so we can check for messages without blocking the thread
        u_long iMode = 1;
        if (ioctlsocket(clientSocket, FIONBIO, &iMode) != NO_ERROR) {
            std::cout << "[ERROR] Failed to set non-blocking mode." << std::endl;
            closesocket(clientSocket);
            continue;
        }

        // Sending data to the connected client
        while (isRunning) {
            // Increment loop iteration count for metrics
            metrics.loopIterations.fetch_add(1, std::memory_order_relaxed);

            // Recieve Data
            std::string buffer(1024, 0);
            int recvResult = recv(clientSocket, buffer.data(), buffer.size(), 0);
            if (recvResult > 0) {
                // Command received, process it
                buffer.resize(recvResult); 
                std::cout << "[REACT] " << buffer << std::endl;
            } 
            else if (recvResult == 0) {
                std::cout << "[DEBUG] React UI disconnected." << std::endl;
                break;
            } 
            else {
                int err = WSAGetLastError();
                if (err != WSAEWOULDBLOCK) {
                    std::cout << "[DEBUG] Read error / lost connection: " << err << std::endl;
                    break;
                }
            }

            // Send Data
            std::string messageToSend = "Engine Step Data\n";
            int sendResult = send(clientSocket, messageToSend.c_str(), messageToSend.length(), 0);

            if (sendResult == SOCKET_ERROR) {
                int err = WSAGetLastError();
                if (err == WSAEWOULDBLOCK) {
                    // OS buffer is full, drop this frame but keep connection alive
                    metrics.failedSends.fetch_add(1, std::memory_order_relaxed);
                } else {
                    // Fatal error, drop client
                    std::cout << "[DEBUG] Send fatal error: " << err << std::endl;
                    break; 
                }
            } else {
                metrics.messagesSent.fetch_add(1, std::memory_order_relaxed);
                metrics.bytesTransmitted.fetch_add(sendResult, std::memory_order_relaxed);
            }

            // ~60hz
            Sleep(16);
        }

        // Close the client socket
        closesocket(clientSocket);
    }
}

void WebSocketServer::debugLoop() {
    uint64_t lastMessages = 0;
    uint64_t lastBytes = 0;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Read the current state of the atomics
        uint64_t currentMessages = metrics.messagesSent.load(std::memory_order_relaxed);
        uint64_t currentBytes = metrics.bytesTransmitted.load(std::memory_order_relaxed);
        uint32_t failures = metrics.failedSends.load(std::memory_order_relaxed);

        // Calculate rates
        uint64_t messagesPerSec = currentMessages - lastMessages;
        uint64_t bytesPerSec = currentBytes - lastBytes;

        std::cout << "[DEBUG] Net: " 
                  << messagesPerSec << " msg/s | " 
                  << (bytesPerSec / 1024) << " KB/s | "
                  << "Fails: " << failures << std::endl;

        lastMessages = currentMessages;
        lastBytes = currentBytes;

        // If messagesPerSec drops to 0 while the simulation is supposedly 
        // running, trigger a warning! You just detected a hung network thread.
    }
}