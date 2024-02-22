#include <iostream>
#include <WinSock2.h>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include <ws2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

class Client {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    bool running;

    void receiveMessages() {
        char buffer[4096];
        while (running) {
            ZeroMemory(buffer, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived > 0) {
                std::cout << "\n SERVER> " << std::string(buffer, 0, bytesReceived) << std::endl;
            }
            else {
                running = false;
            }
        }
    }

public:
    Client() : clientSocket(INVALID_SOCKET), running(false) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            exit(1);
        }
    }

    ~Client() {
        running = false;
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        WSACleanup();
    }

    bool connectServer(const std::string& ip, int port) {
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            closesocket(clientSocket);
            return false;
        }

        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            return false;
        }

        std::cout << "Connected to server." << std::endl;

        running = true;
        std::thread listenerThread(&Client::receiveMessages, this);
        listenerThread.detach();

        return true;
    }

    void sendCommand(const std::string& command) {
        send(clientSocket, command.c_str(), command.size(), 0);
    }

    void stop() {
        running = false;
    }
};


int main() {
    Client client;
    if (client.connectServer("127.0.0.1", 12345)) {
        std::string command;
        while (true) {
            std::cout << "Enter command: ";
            std::getline(std::cin, command);
            if (command == "exit") {
                client.stop();
                break;
            }
            client.sendCommand(command);
        }
    }
    return 0;
}
