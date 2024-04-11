/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:11 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/10 19:59:08 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	struct timeval	time_i;
	struct timeval	time_f;
	
	gettimeofday(&time_i, NULL);
	gettimeofday(&time_f, NULL);
	while ((int)(time_f.tv_usec - time_i.tv_usec) < 200)
		gettimeofday(&time_f, NULL);
	printf("%lu\n", (long unsigned)(time_f.tv_usec - time_i.tv_usec));
	return (0);
}

int	main(int argc, char **argv)
{
	int				i;
	int				*args_array;
	static t_args	args;
	t_philo			*philo;

	if (argc != 5 && argc != 6)
		return (0);
	args_array = (int *)&args;
	i = 1;
	while (i < argc)
	{
		args_array[i - 1] = atoi(argv[i]);
		i++;
	}
	i = 1;
	while (i < 5)
	{
		if (args_array[i - 1] == 0)
			return (0);
		i++;
	}
	philo = malloc(args.n_philo * sizeof(t_philo));

	pthread_t	thread;
	pthread_create(&thread, NULL, routine, NULL);
	pthread_join(thread, NULL);
	return (0);
}
