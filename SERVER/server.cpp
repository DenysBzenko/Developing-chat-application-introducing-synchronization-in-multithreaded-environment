#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <filesystem>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Room {
    vector<SOCKET> clients;
};

class Server {
private:
    SOCKET serverSocket;
    sockaddr_in serverAddr;
    unordered_map<SOCKET, string> clientRooms;
    unordered_map<string, Room> rooms;
    mutex roomsMutex;

public:
    Server() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "WSAStartup failed." << endl;
            exit(1);
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            cerr << "Socket creation failed." << endl;
            WSACleanup();
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(12345);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Bind failed." << endl;
            closesocket(serverSocket);
            WSACleanup();
            exit(1);
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            cerr << "Listen failed." << endl;
            closesocket(serverSocket);
            WSACleanup();
            exit(1);
        }
    }

    ~Server() {
        closesocket(serverSocket);
        WSACleanup();
    }

    void run() {
        cout << "Server is running, waiting for connections..." << endl;
        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                cerr << "Accept failed." << endl;
                continue;
            }

            thread(&Server::handleClient, this, clientSocket).detach();
        }
    }

private:
    void handleClient(SOCKET clientSocket) {
        char buffer[4096];

        while (true) {
            ZeroMemory(buffer, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, 4096, 0);
            if (bytesReceived <= 0) {
                break;
            }

            string request(buffer, 0, bytesReceived);
            processRequest(clientSocket, request);
        }

        cout << "Client disconnected." << endl;
        closesocket(clientSocket);
    }

    void processRequest(SOCKET clientSocket, const string& request) {
        stringstream ss(request);
        string command;
        ss >> command;

        if (command == "CREATE_ROOM") {
            string roomName;
            ss >> roomName;
            createRoom(clientSocket, roomName);
        }
        else if (command == "JOIN_ROOM") {
            string roomName;
            ss >> roomName;
            joinRoom(clientSocket, roomName);
        }
        else if (command == "LEAVE_ROOM") {
            leaveRoom(clientSocket);
        }
        else if (command == "SEND_MESSAGE") {
            string roomName;
            ss >> roomName;
            string message;
            getline(ss, message);
            sendMessage(clientSocket, roomName, message);
        }
        
        else if (command == "JOIN_ANOTHER_ROOM") {
            string roomName;
            ss >> roomName;
            leaveRoom(clientSocket);
            joinRoom(clientSocket, roomName);
            
        }
        else {
            cout << "Unknown command received." << endl;
        }
    }

    void createRoom(SOCKET clientSocket, const string& roomName) {
        lock_guard<mutex> lock(roomsMutex);
        if (rooms.find(roomName) == rooms.end()) {
            rooms[roomName] = Room();
            sendResponse(clientSocket, "Room created successfully");
        }
        else {
            sendResponse(clientSocket, "Room already exists");
        }
    }

    void joinRoom(SOCKET clientSocket, const string& roomName) {
        lock_guard<mutex> lock(roomsMutex);
        auto it = rooms.find(roomName);
        if (it != rooms.end()) {
            it->second.clients.push_back(clientSocket);
            clientRooms[clientSocket] = roomName;
            sendResponse(clientSocket, "Joined room successfully");
        }
        else {
            sendResponse(clientSocket, "Room does not exist");
        }
    }

    void leaveRoom(SOCKET clientSocket) {
        lock_guard<mutex> lock(roomsMutex);
        auto it = clientRooms.find(clientSocket);
        if (it != clientRooms.end()) {
            string roomName = it->second;
            auto& clients = rooms[roomName].clients;
            clients.erase(remove(clients.begin(), clients.end(), clientSocket), clients.end());
            clientRooms.erase(clientSocket);
            sendResponse(clientSocket, "Left room successfully");
        }
    }

    void sendMessage(SOCKET clientSocket, const string& roomName, const string& message) {
        lock_guard<mutex> lock(roomsMutex);

        auto itRoom = clientRooms.find(clientSocket);
        if (itRoom == clientRooms.end() || itRoom->second != roomName) {
            sendResponse(clientSocket, "Error: You are not in the room you are trying to send a message to.");
            return;
        }

        auto it = rooms.find(roomName);
        if (it == rooms.end()) {
            sendResponse(clientSocket, "Error: Room does not exist.");
            return;
        }

        for (auto& socket : it->second.clients) {
            if (socket != clientSocket) {
                sendResponse(socket, "Message from room " + roomName + ": " + message);
            }
        }
    }

    void sendResponse(SOCKET clientSocket, const string& message) {
        send(clientSocket, message.c_str(), message.length(), 0);
    }
};

int main() {
    Server server;
    server.run();
    return 0;
}
