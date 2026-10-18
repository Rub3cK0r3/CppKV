// This module interacts directly with the host OS syscalls 
// to manage low-level I/O. Its initial purpose is to 
// manage File Descriptors, listen on specific ports, and pipeline 
// the binary data streams into the storage.

#ifndef SERVER_PROCESS_H
#define SERVER_PROCESS_H

#include <cstdint>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

struct Request {
    int clientFd;                    // Client socket File Descriptor originating the request
    std::string method;              // HTTP method or protocol command (e.g., "GET", "SET")
    std::string key;                 // Target key associated with the storage operation
    std::vector<uint8_t> rawBody;    // Incoming raw binary data stream
};

struct Response {
    int clientFd;                    // Destination client socket File Descriptor for the response
    uint16_t statusCode;             // Protocol status code (e.g., 200 OK, 404 Not Found)
    std::vector<uint8_t> rawBody;    // Outgoing raw binary data stream to be sent over the socket
};

struct ServerProcess {
    int serverFd;                    // Listening server socket File Descriptor
    int port;                        // Port number assigned to the listening process
    bool isRunning;                  // Execution flag controlling the main server loop

    // Initializes the server instance on a specific port and binds it to the network
    bool start(int targetPort) {
        port = targetPort;
        
        // Create TCP socket descriptor
        serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd < 0) return false;

        // Fast socket recycling
        int opt = 1;
        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        // Address configuration
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind and listen syscalls
        if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
        if (listen(serverFd, 10) < 0) return false;

        isRunning = true;
        return true;
    }

    // Reads the incoming binary stream from a client socket using the recv() syscall
    Request readRequest(int clientSocketFd) {
        Request req;
        req.clientFd = clientSocketFd;

        uint8_t buffer[1024]; // Basic 1KB stack buffer
        ssize_t bytesRead = recv(clientSocketFd, buffer, sizeof(buffer), 0);
        
        if (bytesRead > 0) {
            req.rawBody.assign(buffer, buffer + bytesRead);
            
            // MVP Protocol Parser: expect exact format "CMD KEY"
            std::string rawStr(reinterpret_cast<char*>(buffer), bytesRead);
            size_t space = rawStr.find(' ');
            if (space != std::string::npos) {
                req.method = rawStr.substr(0, space);
                req.key = rawStr.substr(space + 1);
                // Quick strip for text-protocol payloads (\r or \n)
                while (!req.key.empty() && (req.key.back() == '\n' || req.key.back() == '\r')) {
                    req.key.pop_back();
                }
            }
        }
        return req;
    }
    // Flushes the binary payload back to the operating system using the send() syscall
    void sendResponse(const Response& res) {
        if (!res.rawBody.empty()) {
            send(res.clientFd, res.rawBody.data(), res.rawBody.size(), 0);
        }
    }
    // Gracefully closes the listening server socket
    void stop() {
    if (isRunning) {
        isRunning = false;
        close(serverFd);
    }
}

};

#endif // SERVER_PROCESS_H

