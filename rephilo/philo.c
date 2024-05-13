/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:52:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/13 15:20:38 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//N_EAT NOT WORKING

#include "philo.h"

int	main(int argc, char **argv)
{
	int				i;
	static size_t	args[5];
	t_philo			*philos;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	*m_grab;
	pthread_mutex_t	*m_print;
	pthread_mutex_t	*m_monitor;
	size_t			ms;
	int				*can_print;

	if (argc != 5 && argc != 6)
		return (0);
	i = 0;
	while (i++ < argc - 1)
	{
		args[i - 1] = atoi(argv[i]); //ATOI
		if (i != 5 && args[i - 1] == 0)
			return (0);
	}
	philos = malloc(args[N_PHILO] * sizeof(t_philo));
	m_forks = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	m_grab = malloc(sizeof(pthread_mutex_t));
	m_print = malloc(sizeof(pthread_mutex_t));
	m_monitor = malloc(sizeof(pthread_mutex_t));
	can_print = malloc(sizeof(int));
	i = 0;
	*can_print = 1;
	pthread_mutex_init(m_print, NULL);
	pthread_mutex_init(m_monitor, NULL);
	pthread_mutex_init(m_grab, NULL);
	while (i < args[N_PHILO])
	{
		philos[i].num = i;
		philos[i].args = args;
		philos[i].m_rfork = m_forks + i;
		philos[i].m_print = m_print;
		philos[i].m_monitor = m_monitor;
		philos[i].eat_count = 0;
		philos[i].can_print = can_print;
		philos[i].m_grab = m_grab;
		if (i > 0)
			philos[i].m_lfork = m_forks + i - 1;
		pthread_mutex_init(m_forks + i, NULL);
		i++;
	}
	philos[0].m_lfork = m_forks + args[N_PHILO] - 1;
	i = 0;
	ms = ft_getmsofday();
	while (i < args[N_PHILO])
	{
		philos[i].t0 = ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	usleep(100);
	i = 1;
	while (i < args[N_PHILO])
	{
		philos[i].t0 = ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	i = 0;
	if (monitor(philos, m_monitor, m_print, can_print))
	{
		while (i < args[N_PHILO])
			pthread_detach(philos[i++].thread);
	}
	else
	{
		while (i < args[N_PHILO])
			pthread_join(philos[i++].thread, NULL);
	}
	i = 0;
	while (i < args[N_PHILO])
		pthread_mutex_destroy(philos[i++].m_lfork);
	pthread_mutex_destroy(m_print);
	pthread_mutex_destroy(m_monitor);
	pthread_mutex_destroy(m_grab);
	printf("HELLO\n");
	free(philos);
	free(m_forks);
	free(m_print);
	free(m_monitor);
	free(can_print);
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
		pthread_mutex_lock(philo->m_grab);
		if (philo->num)
		{
			pthread_mutex_lock(philo->m_rfork);
			print_action(philo, FORK);
			pthread_mutex_lock(philo->m_lfork);
			print_action(philo, FORK);
		}
		else
		{
			pthread_mutex_lock(philo->m_lfork);
			print_action(philo, FORK);
			pthread_mutex_lock(philo->m_rfork);
			print_action(philo, FORK);
		}
		pthread_mutex_unlock(philo->m_grab);
		print_action(philo, EAT);
		pthread_mutex_lock(philo->m_monitor);
		philo->die_time = ft_getmsofday() - philo->t0 + philo->args[TT_DIE];
		pthread_mutex_unlock(philo->m_monitor);
		ft_msleep(philo->args[TT_EAT]);
		pthread_mutex_lock(philo->m_monitor);
		philo->eat_count++;
		pthread_mutex_unlock(philo->m_monitor);
		pthread_mutex_unlock(philo->m_rfork);
		pthread_mutex_unlock(philo->m_lfork);
		if (philo->eat_count == philo->args[N_EAT])
			return (0);
		print_action(philo, SLEEP);
		ft_msleep(philo->args[TT_SLEEP]);
		print_action(philo, THINK);
	}
	return (0);
}

void	print_action(t_philo *philo, int print_code)
{
	size_t	ms;
	int		*can_print;

	can_print = philo->can_print;
	pthread_mutex_lock(philo->m_print);
	if (*can_print)
	{
		ms = ft_getmsofday();
		if (print_code == FORK)
			printf("%5lu %3i has taken a fork\n", ms - philo->t0, philo->num + 1);
		else if (print_code == EAT)
			printf("%5lu %3i is eating\n", ms - philo->t0, philo->num + 1);
		else if (print_code == SLEEP)
			printf("%5lu %3i is sleeping\n", ms - philo->t0, philo->num + 1);
		else if (print_code == THINK)
			printf("%5lu %3i is thinking\n", ms - philo->t0, philo->num + 1);
	}
	pthread_mutex_unlock(philo->m_print);
}

int	monitor(t_philo *philos, pthread_mutex_t *m_monitor, pthread_mutex_t *m_print, int *can_print)
{
	int	i;
	int	full_tummies;

	i = 0;
	full_tummies = 1;
	while (1)
	{
		pthread_mutex_lock(m_monitor);
		if (ft_getmsofday() - philos[i].t0 >= philos[i].die_time)
		{
			pthread_mutex_unlock(m_monitor);
			pthread_mutex_lock(m_print);
			*can_print = 0;
			printf("%5lu %3i has died\n", ft_getmsofday() - philos[i].t0, philos[i].num + 1);
			pthread_mutex_unlock(m_print);
			return (1);
		}
		if (philos[i].eat_count < philos[i].args[N_EAT])
			full_tummies = 0;
		pthread_mutex_unlock(m_monitor);
		if (i == philos[i].args[N_PHILO] - 1)
		{
			if (philos[i].args[N_EAT] != 0 && full_tummies == 1)
			{
				pthread_mutex_lock(m_print);
				*can_print = 0;
				pthread_mutex_unlock(m_print);
				return (0);
			}
			full_tummies = 1;
			i = 0;
		}
		else
			i++;
	}
}
