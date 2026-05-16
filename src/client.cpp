#include "../include/common.h"
#include <thread>

int sock_fd;

// Thread to receive messages from server
void receive_messages() {
    char buffer[BUFFER_SIZE];
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (bytes <= 0) {
            std::cout << RED << "\nDisconnected from server." << RESET << std::endl;
            exit(0);
        }
        std::cout << std::string(buffer);
        std::cout.flush();
    }
}

int main() {
    struct sockaddr_in server_addr;

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << RED << "Socket creation failed!" << RESET << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to server
    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << RED << "Connection to server failed!" << RESET << std::endl;
        return -1;
    }

    std::cout << GREEN << "=== Connected to Chat Server ===" << RESET << std::endl;

    // Start receiving thread
    std::thread(receive_messages).detach();

    // Send messages
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "/quit") {
            break;
        }
        message += "\n";
        send(sock_fd, message.c_str(), message.size(), 0);
    }

    close(sock_fd);
    return 0;
}