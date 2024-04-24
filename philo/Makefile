NAME = philo

CFLAGS = -Wall -Wextra -Werror

SRCS = philo.c routines.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS) $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(NAME)

.c.o:
	cc $(CFLAGS) -c $<

clean:
	rm $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re
