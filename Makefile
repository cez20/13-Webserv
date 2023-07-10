NAME        = webserv

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = inc

SRC  		= monitorServer.cpp launchServer.cpp ConfigFile.cpp main.cpp HttpRequest.cpp HttpResponse.cpp utils.cpp
INC_FILES   = webserv.hpp HttpRequest.hpp HttpResponse.hpp ConfigFile.hpp

OBJ         = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(SRC)))
INCS        = $(addprefix $(INC_DIR)/,$(INC_FILES))

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98 -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ)  -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinc/ -c $< -o $@

test: 
	$(CC) $(CFLAGS) connectionloop.cpp -o connectionLoop

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)
	rm -rf connectionLoop
	rm -rf webservTests

re: fclean all

.PHONY: all clean fclean re cmake