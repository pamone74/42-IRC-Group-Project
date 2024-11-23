/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 19:29:16 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:51:25 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool work; 


const char* check_port_num(const std::string& port)
 {
  
    for (std::string::size_type i = 0; i < port.size(); ++i)
     {
        if (!std::isdigit(port[i]))
        {
            return "Error, port number contin non digit";
        }
    }
    int conv_port = std::atoi(port.c_str());
    if (conv_port < 1024 || conv_port > 65535) 
    {
        return "Error, port range between 1024 && 65535";
    }
  
    return "";
}


void handle_ctrl_c(int sig) 
{
    (void)sig;
    std::cout << std::endl;
    std::cout << "succes out of program,Ctrl C" << std::endl;
    work = false;
}

int main(int ac, char** av) 
{
    if (ac != 3) 
    {
        std::cerr << "Enter ./ircserv port password" << std::endl;
        return 1;
    }
    
  
    const char* port_error = check_port_num(av[1]);
    if (port_error[0] != '\0') 
    {
        std::cerr << port_error << std::endl;
        return 1;
    }
    
    work = true;
    signal(SIGINT, handle_ctrl_c);

    // Assuming Server is a class defined elsewhere
    Server server(HOST, av[1], av[2]); // Replace HOST with the actual host definition
    //svervr take port numm and pass

    try
    {
        server.initialize_server();
        server.receive();
        std::cout << "Server started and receiving..." << std::endl; // Placeholder
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}