# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/09 07:11:04 by aumartin          #+#    #+#              #
#    Updated: 2025/06/23 21:39:31 by tjacquel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    	= lib.a
LIBFT		= libft/ft_atoi.c\
		libft/ft_atol.c\
		libft/ft_bzero.c\
		libft/ft_calloc.c\
		libft/ft_isalnum.c\
		libft/ft_isalpha.c\
		libft/ft_isascii.c\
		libft/ft_isdigit.c\
		libft/ft_isprint.c\
		libft/ft_itoa.c\
		libft/ft_lstadd_back.c\
		libft/ft_lstadd_front.c\
		libft/ft_lstclear.c\
		libft/ft_lstdelone.c\
		libft/ft_lstiter.c\
		libft/ft_lstlast.c\
		libft/ft_lstmap.c\
		libft/ft_lstnew.c\
		libft/ft_lstsize.c\
		libft/ft_memchr.c\
		libft/ft_memcmp.c\
		libft/ft_memcpy.c\
		libft/ft_memmove.c\
		libft/ft_memset.c\
		libft/ft_putchar_fd.c\
		libft/ft_putendl_fd.c\
		libft/ft_putnbr_fd.c\
		libft/ft_putstr_fd.c\
		libft/ft_split.c\
		libft/ft_strchr.c\
		libft/ft_strdup.c\
		libft/ft_striteri.c\
		libft/ft_strjoin.c\
		libft/ft_strlcat.c\
		libft/ft_strlcpy.c\
		libft/ft_strlen.c\
		libft/ft_strmapi.c\
		libft/ft_strncmp.c\
		libft/ft_strcmp.c\
		libft/ft_strnstr.c\
		libft/ft_strrchr.c\
		libft/ft_strtrim.c\
		libft/ft_substr.c\
		libft/ft_tolower.c\
		libft/ft_toupper.c
PRINTF		= ft_printf/ft_printf.c ft_printf/ft_puthex.c ft_printf/ft_putnbr.c \
	ft_printf/ft_puttxt.c
GNL			= gnl/get_next_line_bonus.c gnl/get_next_line_utils_bonus.c
OBJS		= ${LIBFT:.c=.o} ${PRINTF:.c=.o} ${GNL:.c=.o}
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:${OBJS}
		ar -rcs ${NAME} ${OBJS}

all:    ${NAME}

clean:
		${RM} ${OBJS}
		${RM} ${BONUS_O}

fclean:    clean
		${RM} ${NAME}

re:     fclean all

.PHONY: all clean fclean re bonus
