/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:11 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/13 14:42:01 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	lock_lfork;
pthread_mutex_t	lock_rfork;

int	main(int argc, char **argv)
{
	int				i;
	int				*args_array;
	static t_args	args;
	t_philo			*philos;
	int	*lfork;
	size_t			t0;

	pthread_mutex_init(&lock_lfork, NULL);
	pthread_mutex_init(&lock_rfork, NULL);
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
	lfork = NULL;
	i = 0;
	while (i < args.n_philo)
	{
		philos[i].num = (size_t)(i + 1);
		philos[i].eat_counter = 0;
		philos[i].args = &args;
		philos[i].lfork = lfork;
		lfork = malloc(sizeof(int));
		*lfork = 0;
		philos[i].rfork = lfork;
		// pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	if (args.n_philo > 1)
		philos[0].lfork = lfork;
	else
	{
		philos[0].lfork = malloc(sizeof(int));
		*(philos[0].lfork) = 0;
	}
	i = 0;
	t0 = ft_getmsofday();
	while (i < args.n_philo)
	{
		philos[i].t0 = t0;
		pthread_create(&philos[i].thread_id, NULL, routine, (void *)(&philos[i]));
		usleep(1);
		i++;
	}
	i = 0;
	while (i < args.n_philo)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	pthread_mutex_destroy(&lock_lfork);
	pthread_mutex_destroy(&lock_rfork);
	return (0);
}

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

void	*routine(void *arg)
{
	size_t	ms;
	t_philo	*philo;

	philo = (t_philo *)arg;
	get_forks(philo);
	p_eat(philo);
	p_sleep(philo);
	return (0);
}
