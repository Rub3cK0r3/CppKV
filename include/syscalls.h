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
    bool start(int targetPort);
    // Reads the incoming binary stream from a client socket using the recv() syscall
    Request readRequest(int clientSocketFd);
    // Flushes the binary payload back to the operating system using the send() syscall
    void sendResponse(const Response& res);
    // Stop and graceful shutdown
    void stop();
};

#endif // SERVER_PROCESS_H

