#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#new

int main()
{
    //std::cout << "Hello\n";
    //create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    //use the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);
    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1){
        std::cerr << "Failed to bind socket." << std::endl;
        close(server_socket);
        return 1;
    }

    //listen for incoming connections
    if (listen(server_socket, 3) == -1){
        std::cerr << "Failed to listen for incoming connections." << std::endl;
        close(server_socket);
        return 1;
    }

    //accept incoming connections
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen);
    if (client_socket == -1) {
        std::cerr << "Failed to accept incoming connection." << std::endl;
        close(server_socket);
        return 1;
    }

    //Receive a file from peer 2 and put into random.txt
    char buffer[1024] = {0}; //might be able to make this as big as 65535, see MTU of TCP
    std::fstream myfile;
    myfile.open("random.txt", (std::ios::out | std::ios::binary | std::ios::trunc) );
    while(true){
        int recv_len = recv(client_socket, buffer, sizeof(buffer),0);
        if(recv_len == -1){
            std::cerr << "Failed to receive message from peer." << std::endl;
            break;
        } else if (recv_len == 0) {
            break;
        }
        myfile.write(buffer,sizeof(buffer));
    }
    myfile.close();    
    std::cout << "File received." << std::endl;

    //close the socket
    close(client_socket);
    close(server_socket);
    return 0;
}
