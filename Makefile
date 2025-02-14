# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shutan <shutan@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 16:52:53 by shutan         #+#    #+#              #
#    Updated: 2025/02/13 16:52:58 by shutan        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = client server
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I./libft
LIBFT = libft/libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C libft

client: client.o $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) client.o -L./libft -lft -o client

server: server.o $(LIBFT)
	$(CC) $(CFLAGS) server.o $(LIBFT) -o server

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -f client.o server.o

fclean: clean
	$(MAKE) -C libft fclean
	rm -f client server

re: fclean all

.PHONY: all clean fclean re 