/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: schennal <schennal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:17:08 by schennal          #+#    #+#             */
/*   Updated: 2024/11/06 20:39:28 by schennal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// int main(void)
// {
//     Server ser;
//     //std::string password = argv[2]; 
    
//     std::cout << "--------Server ----------------" << std::endl;
//     try
//     {
//         signal(SIGINT, Server::SignalHandler);
//         ser.ServerInt();
//         // ser.ServerSocket();
        
//     }catch(const std::exception &err)
//     {
//         ser.CloseFds();
//         std::cerr << err.what() << std::endl; 
//     }
// }





int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is passed (port and password)
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    // Get port and password from arguments
    int port = std::stoi(argv[1]);  // Convert the port argument to an integer
    std::string password = argv[2]; // The password argument is a string

    // Create a Server object
    Server ser;
    std::cout << "--------Server ----------------" << std::endl;

    try {
        // Set up signal handler for SIGINT
        signal(SIGINT, Server::SignalHandler);
        
        // Start the server with the provided port and password
        ser.ServerInt(port, password);
        // Your server should now listen on the given port, with the password provided
        
        // Optionally, you could also call other methods to handle connections, etc.
        // ser.ServerSocket();
    } catch (const std::exception& err) {
        // If an error occurs, clean up resources and print the error
        ser.CloseFds();
        std::cerr << err.what() << std::endl;
    }

    return 0;
}