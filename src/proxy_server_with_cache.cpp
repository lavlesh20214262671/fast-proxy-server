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

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
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