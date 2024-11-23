/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:49:49 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:49:52 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_H
#define SERVER_H

#include "irc.hpp"


class Server
{
    private:
        std::string     _host;
        std::string     _port;
        std::string     _password;
        int             _serverfd;  
        std::vector<pollfd> _fds;
        std::vector<Client*>		_clientsVec; 
        std::vector<Channel*>	_channelsVec; 
	    void				_checkPoll(void);
	    void				_createClient(void);
        void				_messageReceived(int fd);
    public:
        Server(std::string m_host, std::string port, std::string pass);
        void initialize_server(); 
        void receive();
        Client *getClientByFd(int fd);
        std::string getpass();
        std::string getport();
		std::vector<Client *>	getClients(void);
		bool	checkOperators( void );
		void	messageAllclients(std::string message);
		void	messageToServer(std::string msg, int userFd);
		void	deleteClient(int fd);
		Client	*getClientByNick(std::string nick);
		Channel*				getChannelByName(std::string name);
		void	addChannel( Channel *channel );
		int	getServerFd( void );
		~Server();

};
#endif