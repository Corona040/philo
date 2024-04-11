/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 10:50:10 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/10 11:17:41 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>

typedef struct {
	int		id;
	char	*s;
}	t_thread;

void	*routine(void *args)
{
	printf("%s\n", ((t_thread *)args)->s);
	return (args);
}

int	main(int argc, char **argv)
{
	pthread_t		thread;
	pthread_attr_t	attr;
	t_thread		args;

	args.id = 1;
	args.s = argv[1];
	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, routine, &args);
	pthread_join(thread, NULL);
	return (0);
}
