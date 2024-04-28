/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:11 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/28 14:50:00 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int				i;
	int				*args_array;
	static t_args	args;
	t_philo			*philos;
	// size_t			t0;
	pthread_mutex_t	*forks;
	pthread_mutex_t print;
	pthread_mutex_t	*monitor_mutex;

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
	args = (t_args){args_array[0], args_array[1], args_array[2], args_array[3], args_array[4]};
	philos = malloc(args.n_philo * sizeof(t_philo));
	forks = malloc(args.n_philo * sizeof(pthread_mutex_t));
	monitor_mutex = malloc(args.n_philo * sizeof(pthread_mutex_t));
	pthread_mutex_init(&print, NULL);
	i = 0;
	while (i < (int)args.n_philo)
	{
		philos[i].num = (size_t)(i + 1);
		philos[i].eat_count = 0;
		philos[i].args = &args;
		philos[i].rfork = forks + i;
		philos[i].print = &print;
		philos[i].monitor_mutex = monitor_mutex + i;
		pthread_mutex_init(forks + i, NULL);
		pthread_mutex_init(monitor_mutex + i, NULL);
		if (i > 0)
			philos[i].lfork = forks + i - 1;
		i++;
	}
	if (args.n_philo > 1)
		philos[0].lfork = forks + args.n_philo - 1;
	else
		philos[0].lfork = 0;
	i = 0;
	// t0 = ft_getmsofft_getmsofday()y();
	while (i < (int)args.n_philo)
	{
		philos[i].t0 = ft_getmsofday();
		philos[i].die_time = philos[i].args->tt_die;
		pthread_create(&philos[i].thread_id, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	i = 1;
	while (i < (int)args.n_philo)
	{
		philos[i].t0 = ft_getmsofday();
		philos[i].die_time = philos[i].args->tt_die;
		pthread_create(&philos[i].thread_id, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	// MONITOR DEATHS
	monitor(philos, &print);
	// TODO destroy or detach threads
	while (i < (int)args.n_philo)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	while (i < (int)args.n_philo)
	{
		pthread_mutex_destroy(philos[i].monitor_mutex);
		pthread_mutex_destroy(philos[i].lfork);
		pthread_mutex_destroy(philos[i].rfork);
	}
	pthread_mutex_unlock(&print);
	pthread_mutex_destroy(&print);
	return (0);
}

int	monitor(t_philo *philos, pthread_mutex_t *print)
{
	int		i;
	size_t	ms;
	int		full_tummies;

	full_tummies = 1;
	i = 0;
	while (1)
	{
		ms = ft_getmsofday();
		pthread_mutex_lock(philos->monitor_mutex);
		if (philos[i].die_time < ms - philos[i].t0)
		{
			pthread_mutex_lock(print);
			printf("%5i %3i has died\n", (int)ms - (int)philos[i].t0, (int)philos[i].num);
			pthread_mutex_unlock(philos->monitor_mutex);
			return (1);
		}
		if (philos[i].eat_count < philos[i].args->n_eat || philos[i].args->n_eat == 0)
			full_tummies = 0;
		pthread_mutex_unlock(philos->monitor_mutex);
		if (i == (int)philos[i].args->n_philo - 1)
		{
			i = 0;
			if (full_tummies == 1)
				break;
		}
		else
		{
			i++;
			full_tummies = 1;
		}
	}
	return (0);
}

size_t	ft_getmsofday(void)
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
	size_t	t0;

	t0 = ft_getmsofday();
	while (ft_getmsofday() - t0 < ms)
		usleep(500);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->lfork)
			pthread_mutex_lock(philo->lfork);
		if (philo->rfork)
			pthread_mutex_lock(philo->rfork);
		get_forks(philo);
		p_eat(philo);
		if (philo->lfork)
			pthread_mutex_unlock(philo->lfork);
		if (philo->rfork)
			pthread_mutex_unlock(philo->rfork);
		if (philo->eat_count == philo->args->n_eat)
			break ;
		p_sleep(philo);
	}
	return (0);
}
