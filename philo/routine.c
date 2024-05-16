/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:45:01 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 12:45:25 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
