/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                       /      \   /      \      */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:45:01 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 23:46:58 by eco                 \__/   \__/          */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo_sync(philo);
	pthread_create(&philo->life_support, NULL, life_support, (void *)philo);
	while (1)
	{
		get_forks(philo);
		pthread_mutex_lock(philo->m_life);
		philo->die_time = ft_getmsofday() - *philo->t0 + philo->args[TT_DIE];
		pthread_mutex_unlock(philo->m_life);
		if (eat_sleep(philo))
			return (0);
		print_think(philo);
		if (ft_msleep(philo->args[TT_THINK], philo))
			return (0);
	}
	return (0);
}

void	philo_sync(t_philo *philo)
{
	pthread_mutex_lock(philo->m_sync);
	if (*philo->update_ms)
	{
		*philo->update_ms = 0;
		*philo->t0 = ft_getmsofday();
	}
	pthread_mutex_unlock(philo->m_sync);
}

void	get_forks(t_philo *philo)
{
	if (philo->num == 1)
	{
		pthread_mutex_lock(philo->m_lfork);
		print_fork(philo);
		pthread_mutex_lock(philo->m_rfork);
		print_fork(philo);
	}
	else
	{
		pthread_mutex_lock(philo->m_rfork);
		print_fork(philo);
		pthread_mutex_lock(philo->m_lfork);
		print_fork(philo);
	}
}

int	eat_sleep(t_philo *philo)
{
	print_eat(philo);
	if (eating(philo))
		return (1);
	pthread_mutex_lock(philo->m_tummy);
	if (philo->args[N_EAT])
		philo->eat_c++;
	if (philo->args[N_EAT] && philo->eat_c == (int)philo->args[N_EAT])
	{
		pthread_mutex_unlock(philo->m_rfork);
		pthread_mutex_unlock(philo->m_lfork);
		pthread_mutex_unlock(philo->m_tummy);
		pthread_join(philo->life_support, NULL);
		return (1);
	}
	print_sleep(philo);
	pthread_mutex_unlock(philo->m_rfork);
	pthread_mutex_unlock(philo->m_lfork);
	pthread_mutex_unlock(philo->m_tummy);
	if (ft_msleep(philo->args[TT_SLEEP], philo))
	{
		pthread_join(philo->life_support, NULL);
		return (1);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	if (ft_msleep(philo->args[TT_EAT], philo))
	{
		pthread_mutex_unlock(philo->m_rfork);
		pthread_mutex_unlock(philo->m_lfork);
		pthread_join(philo->life_support, NULL);
		return (1);
	}
	return (0);
}
