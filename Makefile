# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cemenjiv <cemenjiv@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/03 17:54:38 by cemenjiv          #+#    #+#              #
#    Updated: 2023/06/12 11:28:48 by cemenjiv         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= webserv 

SRCS_DIR   		= srcs/
SRCS_FILES		= main.cpp
SRCS 			= $(addprefix $(SRCS_DIR), $(SRCS_FILES))

OBJS_DIR		= objs/
OBJS 			= $(patsubst $(SRCS_DIR)%.cpp, $(OBJS_DIR)%.o, $(SRCS))

CC				= c++
CFLAGS			= -Wall -Wextra -Werror -g -std=c++98
RM				= rm -f 

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
				$(CC) ${CFLAGS}  -c $< -o $@
	
all:			objs $(NAME)

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) ${OBJS} -o $@ 
			
clean:		
				@rm -rf $(OBJS)
				@rm -rf $(OBJS_DIR)
				
fclean:			clean
				$(RM) $(NAME)
				
re:				fclean all

objs:
				@mkdir -p objs

.PHONY:			all clean fclean re 
