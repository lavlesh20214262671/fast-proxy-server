#include "proxy_parse.hpp"
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

class ProxyServerWithCache {
public:
    ProxyServerWithCache(int port) : server_port(port) {}
    void run();

private:
    int server_port;
    void handleClient(SOCKET client_socket);
};

void ProxyServerWithCache::handleClient(SOCKET client_socket) {
    std::cout << "Handling client with caching..." << std::endl;

    char buffer[8192];
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        std::cerr << "No data received from client." << std::endl;
        closesocket(client_socket);
        return;
    }

    std::string requestData(buffer, bytesReceived);

    auto parsed = ParsedRequest::parse(requestData);
    if (!parsed) {
        std::cerr << "Failed to parse request" << std::endl;
        closesocket(client_socket);
        return;
    }

    // Extract host header
    std::string host;
    for (const auto& header : parsed->headers) {
        if (header.key == "Host") {
            size_t colon = header.value.find(':');
            host = (colon != std::string::npos) ? header.value.substr(0, colon) : header.value;
            break;
        }
    }

    if (host.empty()) {
        std::cerr << "Host header missing." << std::endl;
        closesocket(client_socket);
        return;
    }

    addrinfo hints = {}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(host.c_str(), "80", &hints, &res);
    if (status != 0 || !res) {
        std::cerr << "DNS resolution failed: " << gai_strerror(status) << std::endl;
        closesocket(client_socket);
        return;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket" << std::endl;
        freeaddrinfo(res);
        closesocket(client_socket);
        return;
    }

    if (connect(server_socket, res->ai_addr, res->ai_addrlen) < 0) {
        std::cerr << "Failed to connect to target server" << std::endl;
        freeaddrinfo(res);
        closesocket(server_socket);
        closesocket(client_socket);
        return;
    }

    freeaddrinfo(res);

    std::string outbound = parsed->unparse();
    send(server_socket, outbound.c_str(), outbound.size(), 0);

    while ((bytesReceived = recv(server_socket, buffer, sizeof(buffer), 0)) > 0) {
        send(client_socket, buffer, bytesReceived, 0);
    }

    closesocket(server_socket);
    closesocket(client_socket);
}

void ProxyServerWithCache::run() {
    WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaerr != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaerr << std::endl;
        return;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server_port);

    if (bind(server_fd, (sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    listen(server_fd, SOMAXCONN);
    std::cout << "Proxy Server with Cache is running on port " << server_port << std::endl;

    while (true) {
        SOCKET new_socket = accept(server_fd, NULL, NULL);
        if (new_socket != INVALID_SOCKET) {
            std::thread(&ProxyServerWithCache::handleClient, this, new_socket).detach();
        }
    }

    closesocket(server_fd);
    WSACleanup();
}

int main() {
    ProxyServerWithCache proxy(8081); // Change port if needed
    proxy.run();
    return 0;
}
