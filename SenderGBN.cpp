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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <queue>
#include <string>
#include <thread>

using namespace std;

int UNACKED = 0;

void process_ack(int socket_fd)
{
    char recv_buffer[1024];
    int bytes = recvfrom(socket_fd, recv_buffer, sizeof(recv_buffer), 0, NULL, NULL);
    if (bytes < 0)
    {
        cout << "Error receiving packet" << std::endl;
        exit(1);
    }
    int ack = recv_buffer[0];
    if (ack == UNACKED)
    {
        UNACKED++;
    }
}
void gen_packets(queue<string> &buffer, int MAX_BUFFER_SIZE, float PACKET_GEN_RATE, int PACKET_SIZE)
{
    while (1)
    {
        for (int i = 0; i < PACKET_GEN_RATE; i++)
        {
            if (buffer.size() < MAX_BUFFER_SIZE)
            {
                string packet = "";
                for (int i = 0; i < PACKET_SIZE; i++)
                {
                    packet += "a";
                }
                buffer.push(packet);
            }
        }
        sleep(1);
    }
}

int main(int argc, char **argv)
{

    const int MAX_PACKETS = 5;
    const int PACKET_SIZE = 128;
    const float PACKET_GEN_RATE = 0.1;
    const int PORT_NUM = 8080;
    const bool debug = true;
    const int WINDOW_SIZE = 3;
    const int MAX_BUFFER_SIZE = 10;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        cout << "Error creating socket" << std::endl;
        exit(1);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT_NUM);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    thread ack_thread(process_ack, socket_fd);
    queue<string> buffer;
    thread gen_thread(gen_packets, buffer, MAX_BUFFER_SIZE, PACKET_GEN_RATE, PACKET_SIZE);
    while (1)
    {
        string packet = buffer.front();
        int bytes = sendto(socket_fd, packet.c_str(), packet.length(), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (bytes < 0)
        {
            cout << "Error sending packet" << std::endl;
            exit(1);
        }
    }
}