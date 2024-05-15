/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                         /      \   /      \      */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:52:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/15 23:58:17 by eco                 \__/   \__/          */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int				i;
	static size_t	args[5];
	t_philo			*philos;
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	*m_tummies;
	t_state			state;

	if (argc != 5 && argc != 6)
		return (0);
	i = 0;
	while (i++ < argc - 1)
	{
		args[i - 1] = atoi(argv[i]); //ATOI
		if (i != 5 && args[i - 1] == 0)
			return (0);
	}
	state.ms = malloc(sizeof(size_t));
	if (args[N_PHILO] == 1)
	{
		*state.ms = ft_getmsofday();
		printf("%5lu %3i has taken a fork\n", ft_getmsofday() - *state.ms, 1);
		ft_msleep(args[TT_DIE], NULL);
		printf("%5lu %3i has died\n", ft_getmsofday() - *state.ms, 1);
		free(state.ms);
		return (0);
	}
	philos = malloc(args[N_PHILO] * sizeof(t_philo));
	m_forks = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	m_tummies = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	state.m_print = malloc(sizeof(pthread_mutex_t));
	state.m_life = malloc(sizeof(pthread_mutex_t));
	state.m_sync = malloc(sizeof(pthread_mutex_t));
	state.can_print = malloc(sizeof(int));
	state.life = malloc(sizeof(int));
	state.update_ms = malloc(sizeof(int));
	i = 0;
	*state.can_print = 1;
	*state.life = 1;
	*state.update_ms = 1;
	pthread_mutex_init(state.m_print, NULL);
	pthread_mutex_init(state.m_life, NULL);
	pthread_mutex_init(state.m_sync, NULL);
	pthread_mutex_lock(state.m_sync);
	while (i < args[N_PHILO])
	{
		philos[i].num = i;
		philos[i].args = args;
		philos[i].m_rfork = m_forks + i;
		philos[i].m_tummy = m_tummies + i;
		philos[i].m_print = state.m_print;
		philos[i].m_life = state.m_life;
		philos[i].m_sync = state.m_sync;
		philos[i].eat_count = 0;
		philos[i].can_print = state.can_print;
		philos[i].life = state.life;
		philos[i].update_ms = state.update_ms;
		if (i > 0)
			philos[i].m_lfork = m_forks + i - 1;
		pthread_mutex_init(m_forks + i, NULL);
		pthread_mutex_init(m_tummies + i, NULL);
		i++;
	}
	philos[0].m_lfork = m_forks + args[N_PHILO] - 1;
	i = 0;
	while (i < args[N_PHILO])
	{
		philos[i].t0 = state.ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	pthread_mutex_unlock(state.m_sync);
	usleep(10000);
	i = 1;
	while (i < args[N_PHILO])
	{
		philos[i].t0 = state.ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	i = 0;
	while (i < args[N_PHILO])
		pthread_join(philos[i++].thread, NULL);
	i = 0;
	while (i < args[N_PHILO])
	{
		pthread_mutex_destroy(philos[i++].m_lfork);
		pthread_mutex_destroy(philos[i++].m_tummy);
	}
	pthread_mutex_destroy(state.m_print);
	pthread_mutex_destroy(state.m_life);
	free(philos);
	free(m_forks);
	free(m_tummies);
	free(state.m_print);
	free(state.m_life);
	free(state.can_print);
	free(state.life);
	free(state.ms);
	free(state.update_ms);
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
			usleep(500);
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
	pthread_mutex_lock(philo->m_sync);
	if (*philo->update_ms)
	{
		*philo->update_ms = 0;
		*philo->t0 = ft_getmsofday();
	}
	pthread_mutex_unlock(philo->m_sync);
	pthread_create(&philo->life_support, NULL, life_support, (void *)philo);
	while (1)
	{
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
		print_action(philo, EAT);
		pthread_mutex_lock(philo->m_life);
		philo->die_time = ft_getmsofday() - *philo->t0 + philo->args[TT_DIE];
		pthread_mutex_unlock(philo->m_life);
		if (ft_msleep(philo->args[TT_EAT], philo))
		{
			pthread_mutex_unlock(philo->m_rfork);
			pthread_mutex_unlock(philo->m_lfork);
			pthread_join(philo->life_support, NULL);
			return (0);
		}
		pthread_mutex_lock(philo->m_tummy);
		philo->eat_count++;
		pthread_mutex_unlock(philo->m_rfork);
		pthread_mutex_unlock(philo->m_lfork);
		if (philo->eat_count == philo->args[N_EAT])
		{
			pthread_mutex_unlock(philo->m_tummy);
			pthread_join(philo->life_support, NULL);
			return (0);
		}
		pthread_mutex_unlock(philo->m_tummy);
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

	pthread_mutex_lock(philo->m_print);
	if (*philo->can_print)
	{
		pthread_mutex_unlock(philo->m_print);
		ms = ft_getmsofday();
		if (print_code == FORK)
		{
			pthread_mutex_lock(philo->m_print);
			printf("%5lu %3i has taken a fork\n", ms - *philo->t0, philo->num + 1);
			pthread_mutex_unlock(philo->m_print);
		}
		else if (print_code == EAT)
		{
			pthread_mutex_lock(philo->m_print);
			printf("%5lu %3i is eating\n", ms - *philo->t0, philo->num + 1);
			pthread_mutex_unlock(philo->m_print);
		}
		else if (print_code == SLEEP)
		{
			pthread_mutex_lock(philo->m_print);
			printf("%5lu %3i is sleeping\n", ms - *philo->t0, philo->num + 1);
			pthread_mutex_unlock(philo->m_print);
		}
		else if (print_code == THINK)
		{
			pthread_mutex_lock(philo->m_print);
			printf("%5lu %3i is thinking\n", ms - *philo->t0, philo->num + 1);
			pthread_mutex_unlock(philo->m_print);
		}
	}
	else
		pthread_mutex_unlock(philo->m_print);
}

void	*life_support(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->m_tummy);
	while (philo->eat_count < philo->args[N_EAT])
	{
		pthread_mutex_unlock(philo->m_tummy);
		pthread_mutex_lock(philo->m_life);
		if (ft_getmsofday() - *philo->t0 >= philo->die_time)
		{
			*philo->life = 0;
			pthread_mutex_unlock(philo->m_life);
			pthread_mutex_lock(philo->m_print);
			if (*philo->can_print)
				printf("%5lu %3i has died\n", ft_getmsofday() - *philo->t0, philo->num + 1);
			*philo->can_print = 0;
			pthread_mutex_unlock(philo->m_print);
			return (0);
		}
		pthread_mutex_unlock(philo->m_life);
		pthread_mutex_lock(philo->m_tummy);
	}
	pthread_mutex_unlock(philo->m_tummy);
	return (0);
}
