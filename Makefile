NAME        = Webserv

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = inc

SERVER_SRC  = server.cpp ServerConfiguration.cpp main.cpp HttpRequest.cpp HttpResponse.cpp utils.cpp
INC_FILES   = webserv.hpp ServerConfiguration.hpp HttpRequest.hpp HttpResponse.hpp

SERVER_OBJ  = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(SERVER_SRC)))
INCS        = $(addprefix $(INC_DIR)/,$(INC_FILES))

CC          = c++
CFLAGS      = -Wall -Wextra -Werror -std=c++98

SERVER_EXEC = server.exe


all: $(SERVER_EXEC)

$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER_EXEC)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(SERVER_EXEC)

re: fclean all

.PHONY: all clean fclean re