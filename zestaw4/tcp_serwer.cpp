#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 2020
#define BUFFER_SIZE 1026 // 1024 for data + 2 for "\r\n"
#define MAX_EVENTS 10

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
    if (line.front() == ' ' || line.back() == ' ') {
        return false;
    }
    if (line.find("  ") != std::string::npos) {
        return false;
    }
    return std::all_of(line.begin(), line.end(), [](char c) {
        return isalpha(c) || c == ' ';
    });
}

void processClientData(int clientSocket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 2, 0);

    if (bytesRead <= 0) {
        // Client closed connection or error occurred.
        close(clientSocket);
        return;
    }

    std::string data(buffer, bytesRead);
    std::string response;

    if (!isLineValid(data)) {
        response = "ERROR\r\n";
    } else {
        std::istringstream iss(data);
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

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("ERROR socket()");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

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

    int epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("ERROR epoll_create1()");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = serverSocket;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, serverSocket, &ev) == -1) {
        perror("ERROR epoll_ctl: serverSocket");
        exit(EXIT_FAILURE);
    }

    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("ERROR epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == serverSocket) {
                clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
                if (clientSocket < 0) {
                    perror("ERROR accept()");
                    continue;
                }
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientSocket;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, clientSocket, &ev) == -1) {
                    perror("ERROR epoll_ctl: clientSocket");
                    exit(EXIT_FAILURE);
                }
            } else {
                processClientData(events[n].data.fd);
            }
        }
    }

    close(serverSocket);
    return 0;
}
