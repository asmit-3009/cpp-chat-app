/*
#include "../include/common.h"
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

// Global list of connected client sockets
std::vector<int> clients;
std::mutex clients_mutex;

// Broadcast message to all clients except the sender
void broadcast(const std::string& message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client_fd : clients) {
        if (client_fd != sender_fd) {
            send(client_fd, message.c_str(), message.size(), 0);
        }
    }
}

// Handle each client in a separate thread
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];

    // Ask for username
    std::string prompt = "Enter your username: ";
    send(client_fd, prompt.c_str(), prompt.size(), 0);

    memset(buffer, 0, BUFFER_SIZE);
    int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes <= 0) {
        close(client_fd);
        return;
    }

    std::string username(buffer);
    // Remove newline characters
    username.erase(remove(username.begin(), username.end(), '\n'), username.end());
    username.erase(remove(username.begin(), username.end(), '\r'), username.end());

    std::string join_msg = YELLOW + username + " has joined the chat!" + RESET + "\n";
    std::cout << join_msg;
    broadcast(join_msg, client_fd);

    // Keep receiving messages
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            std::string leave_msg = RED + username + " has left the chat." + RESET + "\n";
            std::cout << leave_msg;
            broadcast(leave_msg, client_fd);
            break;
        }

        std::string message = CYAN + username + ": " + RESET + std::string(buffer) + "\n";
        std::cout << message;
        broadcast(message, client_fd);
    }

    // Remove client from list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
    }
    close(client_fd);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << RED << "Socket creation failed!" << RESET << std::endl;
        return -1;
    }

    // Set socket options
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << RED << "Bind failed!" << RESET << std::endl;
        return -1;
    }

    // Listen
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cerr << RED << "Listen failed!" << RESET << std::endl;
        return -1;
    }

    std::cout << GREEN << BOLD << "=== Chat Server Started on PORT " << PORT << " ===" << RESET << std::endl;
    std::cout << YELLOW << "Waiting for clients to connect..." << RESET << std::endl;

    // Accept clients in a loop
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) continue;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_fd);
        }

        std::cout << GREEN << "New client connected! Socket: " << client_fd << RESET << std::endl;

        // Spawn a thread for this client
        std::thread(handle_client, client_fd).detach();
    }

    close(server_fd);
    return 0;
}
    */



#include "../include/common.h"
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <set>

std::vector<int> clients;
std::mutex clients_mutex;

void broadcast(const std::string& message, int sender_fd) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client_fd : clients) {
        if (client_fd != sender_fd) {
            send(client_fd, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];

    std::string prompt = "Enter your username: ";
    send(client_fd, prompt.c_str(), prompt.size(), 0);

    memset(buffer, 0, BUFFER_SIZE);
    int bytes = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (bytes <= 0) {
        close(client_fd);
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
        return;
    }

    std::string username(buffer, bytes);
    username.erase(remove(username.begin(), username.end(), '\n'), username.end());
    username.erase(remove(username.begin(), username.end(), '\r'), username.end());

    std::string join_msg = YELLOW + username + " has joined the chat!" + RESET + "\n";
    std::cout << join_msg;
    broadcast(join_msg, client_fd);

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            std::string leave_msg = RED + username + " has left the chat." + RESET + "\n";
            std::cout << leave_msg;
            broadcast(leave_msg, client_fd);
            break;
        }

        std::string raw(buffer, bytes_received);
        // Remove newlines
        raw.erase(remove(raw.begin(), raw.end(), '\n'), raw.end());
        raw.erase(remove(raw.begin(), raw.end(), '\r'), raw.end());

        if (raw.empty()) continue;
        if (raw == "/quit") break;

        std::string message = CYAN + username + ": " + RESET + raw + "\n";
        std::cout << message;
        broadcast(message, client_fd);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(remove(clients.begin(), clients.end(), client_fd), clients.end());
    }
    close(client_fd);
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << RED << "Socket creation failed!" << RESET << std::endl;
        return -1;
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << RED << "Bind failed!" << RESET << std::endl;
        return -1;
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cerr << RED << "Listen failed!" << RESET << std::endl;
        return -1;
    }

    std::cout << GREEN << BOLD << "=== Chat Server Started on PORT " << PORT << " ===" << RESET << std::endl;
    std::cout << YELLOW << "Waiting for clients to connect..." << RESET << std::endl;

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) continue;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_fd);
        }

        std::cout << GREEN << "New client connected! Socket: " << client_fd << RESET << std::endl;
        std::thread(handle_client, client_fd).detach();
    }

    close(server_fd);
    return 0;
}