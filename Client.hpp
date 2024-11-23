/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:54:01 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:54:02 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"


class Channel;
class Client
{
    private:
    	int fd_user;
    	std::string nickN;
    	std::string userN;
    	bool authent;
    	bool oper;
    	std::string realN;
    	std::string hostN;
    	std::string serverN;
    	std::vector<Channel *> chanVec;// i make vector type of it is channel and i call itchanVec

    public:
    Client(int fduser); 
    void Authn(void);
    void recivMessage(std::string msg);
    bool isAuthntec(void);

    void setNickN(std::string nick);
    void setUserN(std::string username);
    void setRealN(std::string realname);
    void setHostN(std::string hostname);
    void setServerN(std::string servername);
    void addChannel(Channel *channel);

    int getFd(void);
    std::string getNickN(void);
    std::string getUserN(void);
    std::string getRealN(void);
    std::string getHostN(void);
    std::string getServerN(void);
    std::vector<Channel *> getChannels(void);
    void rmChannel(Channel *channel);
    void setOper(void);
    bool isOper(void);
    ~Client(void);


};


#endif