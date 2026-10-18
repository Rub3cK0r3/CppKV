// Orquestador del MVP
//

#include "include/syscalls.h"
#include "include/storage.h"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

int main() {
    ServerProcess server;
    // Instantiate our custom storage for string values
    KeyValueStore<std::string> storage(8); 

    if (!server.start(8080)) {
        std::cerr << "CRITICAL: Syscall bind failed\n";
        return 1;
    }
    std::cout << "MVP Engine running on port 8080...\n";

    while (server.isRunning) {
        // Block until a new connection descriptor arrives
        int clientFd = accept(server.serverFd, nullptr, nullptr);
        if (clientFd < 0) continue;

        Request req = server.readRequest(clientFd);
        Response res;
        res.clientFd = clientFd;
        res.statusCode = 200;

        std::string payload;

        // Route commands directly into storage pointers
        if (req.method == "SET") {
            storage.put(req.key, "VALUE_OK"); // Stores mock flag for testing
            payload = "STORED\n";
        } 
        else if (req.method == "GET") {
            auto item = storage.get(req.key);
            payload = (item != nullptr) ? (item->value + "\n") : "NOT_FOUND\n";
        } 
        else {
            payload = "UNKNOWN_COMMAND\n";
        }

        res.rawBody.assign(payload.begin(), payload.end());
        server.sendResponse(res);
        close(clientFd); // Release connection descriptor immediately
    }

    server.stop();
    return 0;
}
