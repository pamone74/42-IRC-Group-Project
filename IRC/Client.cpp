/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:33:00 by pamone            #+#    #+#             */
/*   Updated: 2024/11/03 01:05:00 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

void Client::Connect(const std::string& ip, int port) {
    // Create socket
    _client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_client_fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    // Prepare server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid IP address");
    }

    // Connect to the server
    if (connect(_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Failed to connect to server");
    }
    SetIpAddress(ip);
    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
}

void Client::SendMessage(const std::string& message) {
    if (send(_client_fd, message.c_str(), message.size(), 0) < 0) {
        throw std::runtime_error("Failed to send message");
    }
}

void Client::ReceiveMessage() {
    char buffer[1024] = {0};
    ssize_t bytes_received = recv(_client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received string
        std::cout << "Received from server: " << buffer << std::endl;
    } else if (bytes_received == 0) {
        std::cout << "Server disconnected." << std::endl;
    } else {
        throw std::runtime_error("Error receiving message");
    }
}

void Client::Close() {
    close(_client_fd);
    std::cout << "Connection closed." << std::endl;
}


#include "Client.hpp"

int main() {
    try {
        Client client;
        client.Connect("0.0.0.0", 3030); // Change to  server's IP and port
        client.SendMessage("Hello, Server!");
        client.ReceiveMessage();
        client.Close();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
