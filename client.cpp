#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define DEFAULT_PORT 1700
#define DEFAULT_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool check_for_exit(const char *);


int main (int argc, char *argv[]) {
    int client;

    struct sockaddr_in server_address;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout << "socket error establishing socket error" << std::endl;
        exit (1);
    }

    server_address.sin_port = htons (DEFAULT_PORT);
    server_address.sin_family  = AF_INET;
    inet_pton (AF_INET, SERVER_IP,  &server_address.sin_addr);

    std::cout << "\n => client socket created.";
    int ret = connect(client, reinterpret_cast <const struct sockaddr *>(&server_address),sizeof(server_address));
    if(ret == 0) {
        std::cout <<  "=> connection to server" 
        << inet_ntoa(server_address.sin_addr) 
        <<  "with port number " 
        << DEFAULT_PORT << std::endl;
    }
    std::cout <<  "waiting for server confirmation" << std::endl;
    char buffer[DEFAULT_BUFFER_SIZE]; 

    recv(client, buffer, DEFAULT_BUFFER_SIZE, 0);
    std::cout << "Connection established" << std::endl;
    std::cout << "Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL 
    << "To close the connection";  

    while(true) {
        std::cout << "Client" << std::endl;
        std::cin.getline(buffer, DEFAULT_BUFFER_SIZE);
        send(client, buffer, DEFAULT_BUFFER_SIZE, 0);
        if(check_for_exit(buffer)){
            break;
        }
        std::cout << "Server: ";
        recv(client, buffer, DEFAULT_BUFFER_SIZE,0);
        std::cout << buffer << std::endl;
        if(check_for_exit(buffer)){
            break;
        }
        std::cout << std::endl;
    }
    close(client);
    std::cout << "Goodbye!" << std::endl;

    return 0;
}

bool check_for_exit(const char *msg) {
    for(int i = 0; i < strlen(msg); i++) {
        if(msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}