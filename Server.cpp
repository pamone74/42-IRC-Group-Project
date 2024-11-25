/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 20:42:45 by pamone            #+#    #+#             */
/*   Updated: 2024/11/25 20:42:55 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

Server::Server(std::string host,std::string port, std::string password)
	:_host(host),_port(port), _password(password)
{
    std::cout << GREEN << "Server created.\n" << RESET;
}


/*
server: struct addrinfo that specifies criteria for selecting socket address structures returned by getaddrinfo()
*result: pointer to a linked list of address structures returned by getaddrinfo(). It represents all possible addresses that match the criteria specified in server.
represents the current address information being considered in the loop. It allows you to iterate through and test each potential network address that getaddrinfo() has returned
run:
	allows the server to reuse the address if it’s in a TIME_WAIT state.

server.ai_flags = AI_PASSIVE : 
	 wildcard IP address(address is automatically filled in with the local IP address), the server will listen on all available network interfaces.
Get address info
NULL: the server should listen on any available network interface
this->m_port.c_str():
	server’s port number as a string.

ai_protocol: Protocol to be used (usually 0, which means any protocol associated with the socket type).
*/

void Server::initialize_server()
{
	struct addrinfo server, *result, *tmp; 
	int run = 0;

	memset(&server, 0, sizeof(server));
	server.ai_family = AF_INET; 
	server.ai_socktype = SOCK_STREAM; 
	server.ai_flags = AI_PASSIVE;
	
	int adinfo = getaddrinfo(NULL, this->_port.c_str(), &server, &result);
	
	if (adinfo != 0)
	{
		throw std::runtime_error(RED "Error: Failed to Get the Address info." RESET);
	}
	// Loop through all the results and bind to the first we can
	for (tmp = result; tmp != NULL; tmp = tmp->ai_next)
	{
		this->_serverfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (this->_serverfd == -1)
			continue;
		if (setsockopt(this->_serverfd, SOL_SOCKET, SO_REUSEADDR, &run, sizeof(int)) == -1)
		{
			close(this->_serverfd);
			throw std::runtime_error(RED "Error: Cannot setsockopt." RESET);
		}
		if (bind(this->_serverfd, tmp->ai_addr, tmp->ai_addrlen) < 0)
		{
			std::cout << GREEN << "Failed to Bind." << tmp->ai_addr << RESET;
			close(this->_serverfd);
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	// No address succeeded in binding
	if (tmp == NULL)
	{
		throw std::runtime_error(RED "Error: unable to bind socket to any address." RESET);
	}
	if (listen(this->_serverfd, 10) == -1)
	{
		close(this->_serverfd);
		throw std::runtime_error(RED "Error: unable to listen for connections." RESET);
	}
}

/*
 create an array of pollfd structures
 Each pollfd structure represents a file descriptor and the events you're interested in
 server's socket file descriptor (_serverfd) added to this array first, monitoring it for new client connections.
 POLLIN: Indicates we are interested in reading data
 0: This is where the results of the poll will be stored

 _fds: A vector that stores all the file descriptors that poll() will monitor. The server socket is added here initially.
 _fds: each representing a file descriptor that the server is monitoring.

 iterator pointing to the first element(server's main listening socket) in the _fds vector.
 this->_fds.size(): how many file descriptors the server is monitoring.
 5000: timeout value in milliseconds (5 seconds). If no events are detected within this time frame, poll() will return, allowing the server to continue executing other tasks or check the state of work.
 monitors multiple file descriptors to see if any of them are ready for reading, writing, or have an error condition. It returns the number of file descriptors with events
*/
void Server::receive()
{
	struct pollfd	pollFd = {this->_serverfd, POLLIN, 0};
	std::vector<pollfd>::iterator	it;
	if (fcntl(this->_serverfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(RED "receive: fcntl: " + std::string(strerror(errno)) + RESET);
	this->_fds.push_back(pollFd);
	// std::cout << GREEN << "Server listening on <host>:<port> = [ " << this->m_host  <<" : " <<this->m_port << " ]🔊"<< RESET << std::endl;
	std::cout << std::endl;
    std::cout << "             +-------------------------------+" << std::endl;
    std::cout << "             |      Start Server Process     |" << std::endl;
    std::cout << "             +-------------------------------+" << std::endl;
    std::cout << "                           |" << std::endl;
    std::cout << "             +-------------------------------+" << std::endl;
    std::cout << "             |  Create a Socket (socket())   |" << std::endl;
    std::cout << "             +-------------------------------+" << std::endl;
    std::cout << "                           |" << std::endl;
    std::cout << "           Success                     Failure" << std::endl;
    std::cout << "             |                              |" << std::endl;
    std::cout << "    +------------------+               +-------------------+" << std::endl;
    std::cout << "    | Bind to Address  | <---Retry---> | Exit with Error   |" << std::endl;
    std::cout << "    +------------------+               +-------------------+" << std::endl;
    std::cout << "             |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "    |  Start Listening (listen())   |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "            	|" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "    |  Accept Connection (accept()) |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "            	|" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "    |     Handle Client Requests    |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "            	|" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "    |      Close Client Socket      |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "           	|" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;
    std::cout << "    |      Shutdown Server Loop     |" << std::endl;
    std::cout << "    +-------------------------------+" << std::endl;

    std::cout << std::endl;
	std::cout << GREEN << "Local Host: " << RESET << this->_host << std::endl;
	std::cout << GREEN << "Port Number: " << RESET << this->_port << std::endl;
	std::cout << GREEN << "Password: " << RESET << this->_password << std::endl;
	std::cout << GREEN << "Server listening👂" << RESET << std::endl;

	while (work)
	{
		it = this->_fds.begin();
		if (poll(&(*it), this->_fds.size(), 5000) == -1)
			throw std::runtime_error(RED  "receive: poll: " + std::string(strerror(errno)) + RESET);
		this->_checkPoll();
	}	
}

//Checks if there's an event on the file descriptor (like data to be read).
//POLLIN: there’s data available to be read from the file descriptor (like a message from a client).
//When poll() returns, it updates the revents field of each pollfd struct with the events that actually occurred.
//If the event is on the server socket, it means a new client is trying to connect

void Server::_checkPoll(void)
{
	std::vector<pollfd>::iterator it;
	for (it = this->_fds.begin(); it != this->_fds.end(); it++)
	{
		if (it->revents && POLLIN)
		{
			if (it->fd == this->_serverfd)
				this->_createClient();
			else
				this->_messageReceived(it->fd);			
			break;
		}	
	}
}

/*
clientFd:
	file descriptor for the new client connection.
*newClient:
	pointer to a Client object, which will represent the new client.
cli_addr:
	This structure will store information about the client's address (like IP and port)
length:
	store the size of the cli_addr structure. It's needed by the accept() function

clientFd = accept(this->_serverfd, (struct sockaddr *)&cli_addr, &length);

because accept() needs to know how big the address structure is to correctly fill it with the client's information.
waits for a new client to connect to your server. When a client connects, it creates a new socket (a new file descriptor) specifically for communicating with that client, and returns it as clientFd
@parameters;
this->_serverfd: 
	This is the server's main listening socket.
&cli_addr:
	This is where the client's address information (like IP and port) will be stored.
&len: 
	This tells accept() how much space it has to store the client's address.

pollfd clientPollFd = {clientFd, POLLIN, 0};
	Set the New Client Socket to Non-blocking Mode
	POLLIN you want to know when there is data available to read on the socket.
	0: initializes the revents field to zero.

fcntl()
 - F_SETFL: tells fcntl that we're going to set some flags on the file descriptor.
 - O_NONBLOCK: This flag makes the socket non-blocking, meaning that reading from or writing to the socket will not make the program wait if there's no data.

newClient = new Client(clientFd);
	creates a new Client object to represent the client. The clientFd is passed to the Client constructor,
	Client object knows which file descriptor (which client) it is associated with.
*/
void Server::_createClient(void)
{
	int		clientFd;
	Client	*newClient;
	struct sockaddr_in cli_addr;
	socklen_t	length;
	length = sizeof(cli_addr);
	clientFd = accept(this->_serverfd, (struct sockaddr *)&cli_addr, &length);
	if (clientFd < 0)
		throw std::runtime_error(RED "createClient: accept: " + std::string(strerror(errno)) + RESET);
	pollfd clientPollFd = {clientFd, POLLIN, 0};
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(RED "createClient: fcntl: " + std::string(strerror(errno)) + RESET);

	newClient = new Client(clientFd);
	//Add the New Client and Pollfd to Vectors
	this->_clientsVec.push_back(newClient);
	this->_fds.push_back(clientPollFd);
	std::cout << GREEN <<  "Successfully new client added with fd: " << clientFd << RESET << std::endl;
	return ;
}

void Server::_messageReceived(int fd)
{
	// read data from the socket one character at a time.
	char buffer;
	// accumulate the incoming message from the client.
	std::string string;
	int counter = 0;
	while (string.find("\n"))
	{
		// reads data from the client socket fd into buffer.
		int status = recv(fd, &buffer, sizeof(buffer), 0);
		if (status < 0)
			continue;
		//the client has closed the connection
		else if (status == 0)
		{
			std::cout << GREEN << "Successfully client with fd [ "<< fd << " ] deleted" << RESET << std::endl;
			deleteClient(fd);
			return;
		}
		else
		{
			string += buffer;
			//protect the server from potential infinite loops, excessive memory usage, or corrupted input
			//forces str to a default string to avoid issues.
			if (counter > 500)
				string = "/Error: Input exceeded limit, operation halted.\r\n";
			//If a newline character is found in str, it indicates that a complete command has been received.
			if (string.find("\n") != std::string::npos)
			{
				//string only contains a newline character (\n).
				if (string.size() == 1)
					string = "/Error: Empty command received. No action performed.\r\n";
				Command command(string, fd, *this);
				break;	
			}
		}
	}
}

// Whenever a client provides a file descriptor, this function will be used to look up the corresponding Client object.
//function will return a pointer to a Client
//the function is a member of the Server class and is used to get a client by their file descriptor.
Client *Server::getClientByFd(int fd)
{
	//m_clientVec is a member variable of the Server class, vector containing pointers to Client objects.
	std::vector<Client*>::iterator it = this->_clientsVec.begin();
	for (; it != this->_clientsVec.end(); it++)
	{
		//access the Client pointer it points to.
		//Compares the file descriptor of the current client with the given fd. match=the current client is the one we are looking for.
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);
}

std::string Server::getpass()
{
    return(_password);
}
std::string Server::getport()
{
    return (_port);
}

Client	*Server::getClientByNick(std::string nick)
{

	std::vector<Client *>::iterator	it = this->_clientsVec.begin();

	for ( ; it != this->_clientsVec.end(); it++)
		if ((*it)->getNickN() == nick)
			return (*it);

	return (NULL);

}

std::vector<Client *>	Server::getClients(void)
{
	return (this->_clientsVec);
}

// broadcast a message to all connected clients on the IRC server
void	Server::messageAllclients( std::string message)
{

	std::vector<Client *>::iterator	it = this->_clientsVec.begin();
	// Check if the message ends with a carriage return and newline characters.
	if (message.find("\r\n") == std::string::npos)
		message +="\r\n";
	// Iterate over all connected clients in the vector.
	for( ; it != this->_clientsVec.end(); it++)
	{
		// Send the message to each client .
		if (send((*it)->getFd(), message.c_str(), strlen(message.c_str()), 0) < 0)
		{
			// If sending fails, throw an error with a descriptive message.
            throw std::runtime_error("messageAllclients: send: " + std::string(strerror(errno)));
		}
	}
	return ;
}

// to get messages accross to the server 
void	Server::messageToServer( std::string msg, int userFd ) {

	std::vector<Client *>::iterator	it = this->_clientsVec.begin();

	if (msg.find("\r\n") == std::string::npos)
		msg +="\r\n";

	for( ; it != this->_clientsVec.end(); it++)
		if ((*it)->getFd() != userFd)
			if (send((*it)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
              std::cerr << "messageToServer: send: " << strerror(errno) << std::endl;

	return ;

}

//remove a client from the server completely
void	Server::deleteClient(int fd)
{
	std::vector<Client *>::iterator	clientIt = this->_clientsVec.begin();
	std::vector<Channel *>::iterator	channelIt = this->_channelsVec.begin();
	std::vector<pollfd>::iterator	pollIt = this->_fds.begin();

	// Remove the Client from All Channels
	for ( ; channelIt != this->_channelsVec.end(); channelIt++)
		(*channelIt)->removeUser(getClientByFd(fd));
	// Remove the Client's File Descriptor from the Poll Vector and Close the Socket
	for ( ; pollIt != this->_fds.end(); pollIt++)
	{
		if ((*pollIt).fd == fd)
		{
			this->_fds.erase(pollIt);
			close(fd);
			break ;
		}
	}
	// Remove the Client Object from the Client Vector and Free Memory
	for ( ; clientIt != this->_clientsVec.end(); clientIt++)
	{
		if ((*clientIt)->getFd() == fd)
		{
			delete *clientIt;
			this->_clientsVec.erase(clientIt);
			break ;
		}
	}
}

void	Server::addChannel(Channel *channel)
{
	this->_channelsVec.push_back(channel);
}

int	Server::getServerFd(void)
{
	return (this->_serverfd);
}


// we create the name of the channels for easier lookup later 
Channel*	Server::getChannelByName(std::string name)
{

	std::vector<Channel *>::iterator	it = this->_channelsVec.begin();

	if (name[0] != '#')
		name = "#" + name;

	for ( ; it != this->_channelsVec.end(); it++)
		if ((*it)->getChannelName() == name)
			return (*it);

	return (NULL);

}

bool	Server::checkOperators(void)
{
	std::vector<Client *>::iterator	it = this->_clientsVec.begin();
	for(; it != this->_clientsVec.end(); it++)
		if ((*it)->isOper())
			return true;
	return false;
}

Server::~Server()
{
	std::cout << "desctuctor called!" << std::endl;

	for (size_t i = 0; i < this->_clientsVec.size(); i++)
	{
		delete this->_clientsVec[i];
	}
	for (size_t i = 0; i < this->_channelsVec.size(); i++)
	{
		delete this->_channelsVec[i];
	}
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		close(this->_fds[i].fd);
	}
}
