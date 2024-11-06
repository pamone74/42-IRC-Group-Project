NAME = ircserv
FLAG = -Wall -Wextra -Werror -std=c++98
CC = c++
SRC = Server.cpp main.cpp Client.cpp Channel.cpp Command.cpp

all: $(NAME)

$(NAME) : $(SRC)
	$(CC) $(FLAG) $(SRC) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean:clean

re:fclean all

.PHONY: re all clean fclean