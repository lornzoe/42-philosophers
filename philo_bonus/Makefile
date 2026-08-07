# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/21 01:58:47 by lyanga            #+#    #+#              #
#    Updated: 2026/08/07 15:30:00 by lyanga           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

HEADERDIR = .
SRCS = main_bonus.c init_bonus.c routine_bonus.c monitor_bonus.c \
	   print_bonus.c utils_bonus.c ft_atol_bonus.c
OBJS = $(SRCS:.c=.o)

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c philo_bonus.h
	$(CC) $(CFLAGS) -I${HEADERDIR} -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
