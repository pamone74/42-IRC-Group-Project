# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamone <pamone@student.42abudhabi.ae>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/23 19:28:34 by pamone            #+#    #+#              #
#    Updated: 2024/11/23 19:28:35 by pamone           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
SRC =  Channel.cpp Command.cpp Client.cpp Server.cpp Extra.cpp  main.cpp\
		 Help.cpp Invite.cpp Join.cpp Kick.cpp\
		  Kill.cpp Mode.cpp Nick.cpp Oper.cpp Part.cpp\
		   Pass.cpp Privmsg.cpp Quit.cpp Topic.cpp User.cpp
OBJS = ${SRC:.cpp=.o}

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS)  -o $(NAME)
%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all

