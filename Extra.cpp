/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Extra.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:53:22 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:53:23 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


bool Extra::invalidChar(std::string s)
{
    for(int i = 0; s[i] !='\0'; i++)
    if(!isalnum(s[i]))
    {
        return(true);
	}
    return(false);
}

void Extra::startM(void)
{
    std::string startm;
    startm.append("hello\n");
}

std::vector<std::string> Extra::split(std::string s, char c)
{
    std::string temp = "";
    std::vector<std::string> sp;
    int i =0;
    while(s[i] != '\0')
    {
        if(s[i] != c)
        {
        temp = temp + s[i];
        }
        else if(s[i] == c && temp != "")
        {
        sp.push_back(temp);
        temp ="";
        }
        i++;
    }
     if(temp != "")
      {
        sp.push_back(temp);
      }
        return(sp);

}

std::string Extra::joinSplitItr(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
    std::string msg;
    while(first != last)
    {
        if(!msg.empty())
        {
            msg = msg + " ";
        }
        msg = msg + *first;
        first++;
    }
    return (msg);
}//It can be used with any std::vector<std::string> or similar containers 

std::string Extra::joinSplit(std::vector<Client *> users)
{
    std::string msg;
    std::vector<Client *>::iterator i = users.begin();
    while(i != users.end())
    {
        if(!msg.empty())
        {
            msg = msg + " ";// add space befor appending next users nickname
        }
        msg = msg + (*i)->getNickN();// get nickname from client object and append to msg
        i++; //move to the next client
    }
    return (msg);
}


std::string Extra::toupper(std::string s)
{
    for(int i = 0; s[i] != '\0'; i++)
    s[i] = std::toupper(s[i]);
    return(s);
}
