/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:11 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/12 18:15:44 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int forks;

size_t	ft_getmsofday()
{
	struct timeval	tv;
	size_t			ms;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	ms = (size_t)tv.tv_sec * 1000;
	ms += (size_t)tv.tv_usec / 1000;
	return (ms);
}

int	ft_msleep(size_t ms)
{
	size_t t0;

	t0 = ft_getmsofday();
	while (ft_getmsofday() - t0 < ms)
		usleep(500);
	return (0);
}

pthread_mutex_t	lock;

void	*routine(void *arg)
{
	size_t	ms;
	t_philo	*philo;

	pthread_mutex_lock(&lock);
	philo = (t_philo *)arg;
	ms = ft_getmsofday();
	ft_msleep(philo->args->tt_sleep);
	ms = ft_getmsofday() - ms;
	printf("philo %i slept for %lu\n", (int)philo->num, (long unsigned)ms);
	pthread_mutex_unlock(&lock);
	// getfork();
	// eat();
	// sleep();
	// think();
	return (0);
}

int	main(int argc, char **argv)
{
	int				i;
	int				*args_array;
	static t_args	args;
	t_philo			*philos;

	pthread_mutex_init(&lock, NULL);
	if (argc != 5 && argc != 6)
		return (0);
	args_array = (int *)&args;
	i = 1;
	while (i < argc)
	{
		args_array[i - 1] = atoi(argv[i]); // ATOI
		i++;
	}
	i = 1;
	while (i < 5)
	{
		if (args_array[i - 1] == 0)
			return (0);
		i++;
	}
	args = (t_args){args_array[0], args_array[1], args_array[2], args_array[3]};
	philos = malloc(args.n_philo * sizeof(t_philo));
	i = 0;
	while (i < args.n_philo)
	{
		philos[i].num = (size_t)(i + 1);
		philos[i].args = &args;
		pthread_create(&philos[i].thread_id, NULL, routine, (void *)(&philos[i]));
		usleep(1);
		// pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < args.n_philo)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	pthread_mutex_destroy(&lock);
	return (0);
}
