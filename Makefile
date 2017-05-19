##
## Makefile for  in /home/toune/Documents/Epitech/projets/PSU_2016_myftp
## 
## Made by Thomas HENON
## Login   <thomas.henon@epitech.eu>
## 
## Started on  Thu May 18 10:29:51 2017 Thomas HENON
## Last update Thu May 18 10:29:53 2017 Thomas HENON
##

CC = gcc

RM = rm -f

SRCS =  main.c \
        util.c \
        socket.c \
        socket2.c \
        socket_server.c \
        protocol.c \
        ftp_socket.c \
        ftp_socket2.c \
        ftp_client.c \
        ftp_client2.c \
        ftp_server.c \
        globals.c \
        path.c \
        path2.c \
        ftp_cmd.c \
        ftp_cmd2.c \
        ftp_cmd3.c \
        ftp_data_cmd.c \
        file.c \
        error.c \
        extract_ip_port.c

OBJS = $(SRCS:.c=.o)

NAME = server

LDFLAGS+=

CFLAGS+=

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
