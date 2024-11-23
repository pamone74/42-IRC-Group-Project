/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:54:12 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:54:13 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "irc.hpp"

class Client;

class Channel
{
    private:
        std::vector<Client *> operets;
        std::vector<Client *> usrs_name;
        std::string			channel_name; 
		std::string 		topiC;
		std::string			passworD; 
		std::string			modE;
		unsigned int		limiT;

    public:
        Channel(std::string name,std::string password);
		void				addUser(Client *user);
		void				removeUser(Client *user);
		void				messageFrChannel(std::string msg);
		void				messageToChannel( std::string msg, int senderFd );
        bool				check_mode(char c);
		void				removeOper(Client *user);
        void				addOper(Client *user);

		void 				setTopic(std::string topic);
		void 				setMode(std::string mode);
		void 				setPassword(std::string pass);
        void 				setLimit(unsigned int limit);
		unsigned int		getLimit();
        std::string			getChannelName(void);
		std::string			getPassword(void);
		std::vector<Client *>	getUsersname(void);
        std::string 		getTopic();
        std::string 		getMode();
		Client				*getClientByNick(std::string nick);
        Client				*getOperByNick(std::string nick);
        ~Channel(void);


};



#endif