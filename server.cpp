#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1601
#define DEFAULT_BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool check_for_exit(const char *);

int main(int argc, char *argv[]) {
    int server;
    struct sockaddr_in server_address;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cout << "socket error establishing socket error" << std::endl;
        exit(1);
    }
    std::cout << "Socket has been successfully created" << std::endl;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(DEFAULT_PORT);

    int ret = bind(server, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address));

    if (ret < 0) {
        std::cout << ERROR_S << "Binding connection, socket has already been established" << std::endl;
        return -1;
    }

    socklen_t size = sizeof(server_address);
    std::cout << "SERVER: Listening clients..." << std::endl;
    listen(server, 0);

    int client = accept(server, reinterpret_cast<struct sockaddr *>(&server_address), &size);

    if (client < 0) {
        std::cout << ERROR_S << "Can't accept client" << std::endl;
        return -1;
    }

    char buffer[DEFAULT_BUFFER_SIZE];
    bool isExit = false;
    while (client > 0) {
        strcpy(buffer, "=> server connected");
        send(client, buffer, DEFAULT_BUFFER_SIZE, 0);
        std::cout << "=> Connected to the client #1" << std::endl
                  << "Enter '" << CLIENT_CLOSE_CONNECTION_SYMBOL << "' to end the connection" << std::endl;

        std::cout << "Client: ";
        recv(client, buffer, DEFAULT_BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if (check_for_exit(buffer)) {
            isExit = true;
        }
        while (!isExit) {
            std::cout << "Server: ";
            std::cin.getline(buffer, DEFAULT_BUFFER_SIZE);
            if (check_for_exit(buffer)) {
                break;
            }
            send(client, buffer, DEFAULT_BUFFER_SIZE, 0);
            recv(client, buffer, DEFAULT_BUFFER_SIZE, 0);
            if (check_for_exit(buffer)) {
                break;
            }
            std::cout << "Client: " << buffer << std::endl;
        }
        std::cout << "Goodbye client" << std::endl;
        isExit = false;
    }

    return 0;
}

bool check_for_exit(const char *msg) {
    for (int i = 0; i < strlen(msg); i++) {
        if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}
