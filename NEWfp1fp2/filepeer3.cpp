#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>

int main()
{
    //create a socket
    int psocket = socket(AF_INET, SOCK_STREAM, 0);
    if (psocket == -1){
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    //use the socket
    struct sockaddr_in peer_address;
    peer_address.sin_family = AF_INET;
    peer_address.sin_addr.s_addr = inet_addr("targetip"); //p ip
    peer_address.sin_port = htons(8080);
    if(connect(psocket, (struct sockaddr*)&peer_address, sizeof(peer_address)) == -1){
        std::cerr << "Failed to connect to peer." << std::endl;
        close(psocket);
        return 1;\
    }

    //read raw binary data from random.txt and send file to peer 1
    char buffer[1024] = {0}; //might be able to make this as big as 65535, see MTU of TCP
    std::fstream myfile;
    myfile.open("random.txt", (std::ios::in | std::ios::binary));
    int counter = 0;    
    while(!myfile.eof()){
        myfile.read(buffer,sizeof(buffer));
        if(counter % 100000 == 0){ //print every 100 KB.
            std::cout << counter << "Bytes" << std::endl;
        }
        int send_len = send(psocket, buffer, strlen(buffer), 0);
        if(send_len == -1) {
                std::cerr << "Failed to send message to peer." << std::endl;
            break;
            myfile.close();
        }
        memset(buffer, 0, sizeof(buffer)); //reset buffer
        counter++;
    }
    std::cout << "File sent." << std::endl;
    close(psocket);
    return 0;
}

