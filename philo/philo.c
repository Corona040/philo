/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:52:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/15 18:30:20 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	pthread_mutex_t	*m_life;
	size_t			ms;
	int				*can_print;
	int				*life;

	if (argc != 5 && argc != 6)
		return (0);
	i = 0;
	while (i++ < argc - 1)
	{
		args[i - 1] = atoi(argv[i]); //ATOI
		if (i != 5 && args[i - 1] == 0)
			return (0);
	}
	if (args[N_PHILO] == 1)
	{
		ms = ft_getmsofday();
		printf("%5lu %3i has taken a fork\n", ft_getmsofday() - ms, 1);
		ft_msleep(args[TT_DIE], NULL);
		printf("%5lu %3i has died\n", ft_getmsofday() - ms, 1);
		return (0);
	}
	philos = malloc(args[N_PHILO] * sizeof(t_philo));
	m_forks = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	m_grab = malloc(sizeof(pthread_mutex_t));
	m_print = malloc(sizeof(pthread_mutex_t));
	m_monitor = malloc(sizeof(pthread_mutex_t));
	m_life = malloc(sizeof(pthread_mutex_t));
	can_print = malloc(sizeof(int));
	life = malloc(sizeof(int));
	i = 0;
	*can_print = 1;
	*life = 1;
	pthread_mutex_init(m_print, NULL);
	pthread_mutex_init(m_monitor, NULL);
	pthread_mutex_init(m_life, NULL);
	pthread_mutex_init(m_grab, NULL);
	while (i < args[N_PHILO])
	{
		philos[i].num = i;
		philos[i].args = args;
		philos[i].m_rfork = m_forks + i;
		philos[i].m_print = m_print;
		philos[i].m_monitor = m_monitor;
		philos[i].m_life = m_life;
		philos[i].eat_count = 0;
		philos[i].can_print = can_print;
		philos[i].life = life;
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
	monitor(philos, m_monitor, m_print, m_life, can_print, life);
	while (i < args[N_PHILO])
		pthread_join(philos[i++].thread, NULL);
	i = 0;
	while (i < args[N_PHILO])
		pthread_mutex_destroy(philos[i++].m_lfork);
	pthread_mutex_destroy(m_print);
	pthread_mutex_destroy(m_monitor);
	pthread_mutex_destroy(m_grab);
	pthread_mutex_destroy(m_life);
	free(philos);
	free(m_forks);
	free(m_print);
	free(m_monitor);
	free(m_grab);
	free(m_life);
	free(can_print);
	free(life);
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

int	ft_msleep(size_t ms, t_philo *philo)
{
	size_t	t0;

	t0 = ft_getmsofday();
	if (!philo)
	{
		while (ft_getmsofday() - t0 < ms)
		{
			usleep(500);
		}
	}
	else
	{
		while (ft_getmsofday() - t0 < ms)
		{
			pthread_mutex_lock(philo->m_life);
			if (!*(philo->life))
			{
				pthread_mutex_unlock(philo->m_life);
				return (1);
			}
			pthread_mutex_unlock(philo->m_life);
			usleep(500);
		}
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_create(&philo->life_support, NULL, life_support, (void *)philo);
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
		pthread_mutex_lock(philo->m_life);
		philo->die_time = ft_getmsofday() - philo->t0 + philo->args[TT_DIE];
		pthread_mutex_unlock(philo->m_life);
		if (ft_msleep(philo->args[TT_EAT], philo))
		{
			pthread_mutex_unlock(philo->m_rfork);
			pthread_mutex_unlock(philo->m_lfork);
			pthread_join(philo->life_support, NULL);
			return (0);
		}
		pthread_mutex_lock(philo->m_monitor);
		philo->eat_count++;
		pthread_mutex_unlock(philo->m_monitor);
		pthread_mutex_unlock(philo->m_rfork);
		pthread_mutex_unlock(philo->m_lfork);
		if (philo->eat_count == philo->args[N_EAT])
		{
			pthread_join(philo->life_support, NULL);
			return (0);
		}
		print_action(philo, SLEEP);
		if (ft_msleep(philo->args[TT_SLEEP], philo))
		{
			pthread_join(philo->life_support, NULL);
			return (0);
		}
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

void	*life_support(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(philo->m_life);
		if (ft_getmsofday() - philo->t0 >= philo->die_time)
		{
			*philo->life = 0;
			pthread_mutex_unlock(philo->m_life);
			pthread_mutex_lock(philo->m_print);
			if (*philo->can_print)
				printf("%5lu %3i has died\n", ft_getmsofday() - philo->t0, philo->num + 1);
			*philo->can_print = 0;
			pthread_mutex_unlock(philo->m_print);
			return (0);
		}
		pthread_mutex_unlock(philo->m_life);
	}
}

int	monitor(t_philo *philos, pthread_mutex_t *m_monitor, pthread_mutex_t *m_print, pthread_mutex_t *m_life, int *can_print, int *life)
{
	int	i;
	int	full_tummies;

	i = 0;
	full_tummies = 1;
	while (1)
	{
		pthread_mutex_lock(m_life);
		if (!*life)
		{
			pthread_mutex_unlock(m_life);
			return (1);
		}
		pthread_mutex_unlock(m_life);
		pthread_mutex_lock(m_monitor);
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
