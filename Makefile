NAME        = webserv

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = inc

SRC  = server.cpp ServerConfiguration.cpp main.cpp HttpRequest.cpp HttpResponse.cpp utils.cpp
INC_FILES   = webserv.hpp ServerConfiguration.hpp HttpRequest.hpp HttpResponse.hpp

OBJ         = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(SRC)))
INCS        = $(addprefix $(INC_DIR)/,$(INC_FILES))

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ)  -o $(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinc/ -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re