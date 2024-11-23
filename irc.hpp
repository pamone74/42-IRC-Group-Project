/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 23:52:42 by pamone            #+#    #+#             */
/*   Updated: 2024/11/23 23:52:43 by pamone           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# define HOST "127.0.0.1"
# define MODE "itkol"
extern bool	work;

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <cerrno>
#include <vector>
#include <algorithm>
#include <cctype>
#include <signal.h>
#include <stdexcept>
#include <vector>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define PINK    "\033[95m"
#define CYAN 	"\033[36m"
#define BLUE	"\033[34m"
#define RESET   "\033[0m"

#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Extra.hpp"
#include "Command.hpp"



// info about replynError:
// https://www.alien.net.au/irc/irc2numerics.html

# define OPERATOR_PASS "pamone"


#endif