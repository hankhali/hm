NAME = ircserv
PORT = 8080
PASSWORD = 12345

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iincludes

MAIN = main.cpp
CLIENT = Client.cpp 
SERVER = Server.cpp Cmds.cpp Channel.cpp
UTILS = Utils.cpp Socket.cpp

SRCS = $(addprefix src/, $(MAIN)) \
	$(addprefix src/client/, $(CLIENT)) \
	$(addprefix src/server/, $(SERVER)) \
	$(addprefix src/utils/, $(UTILS))

OBJs = $(SRCS:.cpp=.o)

$(NAME): $(OBJs)
	$(CXX) $(CXXFLAGS) $(OBJs) -o $(NAME)

all: $(NAME)

run: $(NAME) clean
	@./$(NAME) $(PORT) $(PASSWORD)

irssi: rm_irssi
	docker run -it --name my-running-irssi -e TERM -u $(id -u):$(id -g) \
	--log-driver=none \
    -v ${HOME}/.irssi:/home/user/.irssi:ro \
    irssi

rm_irssi:
	@docker rm -f my-running-irssi 2> /dev/null || exit 0

clean:
	@rm -f $(OBJs)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
