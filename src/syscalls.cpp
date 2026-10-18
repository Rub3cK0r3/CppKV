#include "../include/syscalls.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Inicializa la instancia del servidor en un puerto específico y lo vincula a la red
bool ServerProcess::start(int targetPort) {
    port = targetPort;
    
    // Crear descriptor de socket TCP
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) return false;

    // Reciclaje rápido del puerto del socket
    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configuración de la dirección de red
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Vinculación y escucha mediante llamadas al sistema
    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
    if (listen(serverFd, 10) < 0) return false;

    isRunning = true;
    return true;
}

// Lee el flujo binario entrante desde un socket cliente usando recv()
Request ServerProcess::readRequest(int clientSocketFd) {
    Request req;
    req.clientFd = clientSocketFd;

    uint8_t buffer[1024]; // Buffer básico de 1KB en pila
    ssize_t bytesRead = recv(clientSocketFd, buffer, sizeof(buffer), 0);
    
    if (bytesRead > 0) {
        req.rawBody.assign(buffer, buffer + bytesRead);
        
        // Parseador del protocolo MVP: espera el formato exacto "CMD KEY"
        std::string rawStr(reinterpret_cast<char*>(buffer), bytesRead);
        size_t space = rawStr.find(' ');
        if (space != std::string::npos) {
            req.method = rawStr.substr(0, space);
            req.key = rawStr.substr(space + 1);
            
            // Limpieza rápida de saltos de línea (\r o \n) para protocolos de texto
            while (!req.key.empty() && (req.key.back() == '\n' || req.key.back() == '\r')) {
                req.key.pop_back();
            }
        }
    }
    return req;
}

// Envía la carga útil binaria de vuelta al sistema operativo usando send()
void ServerProcess::sendResponse(const Response& res) {
    if (!res.rawBody.empty()) {
        send(res.clientFd, res.rawBody.data(), res.rawBody.size(), 0);
    }
}

// Cierra de forma segura el socket de escucha del servidor
void ServerProcess::stop() {
    if (isRunning) {
        isRunning = false;
        close(serverFd);
    }
}

