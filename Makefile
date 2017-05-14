CC = gcc

RM = rm -f

SRCS =  main.c \
        util.c \
        socket.c \
        socket_server.c \
        protocol.c \
        ftp_socket.c \
        ftp_client.c \
        ftp_server.c \
        globals.c \
        path.c \
        ftp_cmd.c \
        ftp_data_cmd.c

OBJS = $(SRCS:.c=.o)

NAME = server

LDFLAGS+= -g

CFLAGS+= -g

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
