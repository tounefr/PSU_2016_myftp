CC = gcc

RM = rm -f

SRCS =  main.c \
        util.c \
        socket.c \
        protocol.c \
        client.c \
        ftp_socket.c \
        client_cmd.c

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
