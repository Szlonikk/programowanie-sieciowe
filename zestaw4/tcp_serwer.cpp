#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2020
#define BUFFER_SIZE 1026 // 1024 for data + 2 for "\r\n"

bool isPalindrome(const std::string& word) {
    int length = word.length();
    for (int i = 0; i < length / 2; i++) {
        if (tolower(word[i]) != tolower(word[length - i - 1])) {
            return false;
        }
    }
    return true;
}

bool isLineValid(const std::string& line) {
    std::string trimmed = line;
    // Remove trailing newline characters
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), '\r'), trimmed.end());
    trimmed.erase(std::remove(trimmed.begin(), trimmed.end(), '\n'), trimmed.end());

    if (trimmed.find("  ") != std::string::npos) {
        return false;
    }
    return std::all_of(trimmed.begin(), trimmed.end(), [](char c) {
        return isalpha(c) || c == ' ';
    });
}

void handleClient(int clientSocket) {
    std::string buffer;

    while (true) {
        char tempBuffer[BUFFER_SIZE];
        memset(tempBuffer, 0, BUFFER_SIZE);
        ssize_t bytesRead = recv(clientSocket, tempBuffer, BUFFER_SIZE - 2, 0);

        if (bytesRead <= 0) {
            std::cout << "Connection closed or error occurred, closing socket." << std::endl;
            break;
        }

        buffer.append(tempBuffer, bytesRead);

        size_t pos;
        while ((pos = buffer.find("\r\n")) != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 2); 

            std::string response;
            if (!isLineValid(line)) {
                response = "ERROR\r\n";
            } else {
                std::istringstream iss(line);
                std::string word;
                int words = 0, palindromes = 0;
                while (iss >> word) {
                    words++;
                    if (isPalindrome(word)) {
                        palindromes++;
                    }
                }
                std::ostringstream oss;
                oss << palindromes << "/" << words << "\r\n";
                response = oss.str();
            }

            send(clientSocket, response.c_str(), response.length(), 0);
        }
    }

    close(clientSocket);
}


int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("ERROR socket()");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("ERROR bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 100) < 0) {
        perror("ERROR listen()");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running and waiting for connections..." << std::endl;

    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket < 0) {
            perror("ERROR accept()");
            continue;
        }

        std::cout << "Accepted new connection." << std::endl;
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}
