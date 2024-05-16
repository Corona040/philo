NAME = philo

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = philo.c			\
	   philo2.c			\
	   routine.c		\
	   print_action.c	\
	   monitor.c		\
	   time.c			\
	   ft_atoi.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
