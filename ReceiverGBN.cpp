// NAME:
// Roll Number:
// Course: CS3205 Jan. 2023 semester
// Lab number: 2
// Date of submission: <fill>
// I confirm that the source file is entirely written by me without
// resorting to any dishonest means.
// Website(s) that I used for basic socket programming code are:
// URL(s): <fill>

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
int main(int argc, char ** argv) {
    const int MAX_PACKETS = 5;
    int NEXT_EXPECTED = 0;
    const double RANDOM_DROP_VAL = 0.1;
    const int PORT_NUM = 8080;
    const bool debug = true;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        cout << "Error creating socket" << std::endl;
        exit(1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUM);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_fd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Error binding socket" << std::endl;
        exit(1);
    }

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    while(NEXT_EXPECTED < MAX_PACKETS){
        char recv_buffer[1024];
        /* Receive packet from sender */
        /* If packet is corrupt, drop it */ 
        int bytes = recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *) &clientAddress, &clientAddressLength);
        if (bytes < 0) {
            cout << "Error receiving packet" << std::endl;
            exit(1);
        }
        if (rand()/RAND_MAX < RANDOM_DROP_VAL && debug) {
            cout << "Packet dropped" << std::endl;
            continue;
        }
        int seq_no = recv_buffer[0];
        if (seq_no != NEXT_EXPECTED) {
            if(debug) cout << "Packet out of order" << std::endl;
            continue;
        }
        NEXT_EXPECTED++;
        char send_buffer[1024];
        sprintf(send_buffer, "%d", NEXT_EXPECTED);
        int bytes = sendto(socket_fd, send_buffer, sizeof(send_buffer), 0, (struct sockaddr *) &clientAddress, clientAddressLength);
        if (bytes < 0) {
            cout << "Error sending packet" << std::endl;
            exit(1);
        }
    }
    return 0;
}