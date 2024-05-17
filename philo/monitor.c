/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:47:22 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/17 15:43:24 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*life_support(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->m_tummy);
	while (!philo->args[N_EAT] || philo->eat_c < (int)philo->args[N_EAT])
	{
		pthread_mutex_unlock(philo->m_tummy);
		ft_msleep(2, NULL);
		pthread_mutex_lock(philo->m_life);
		if (ft_getmsofday() - *philo->t0 >= philo->die_time)
		{
			if (!philo->args[N_EAT] || philo->eat_c < (int)philo->args[N_EAT])
			{
				philo->is_alive = 0;
				pthread_mutex_unlock(philo->m_life);
				print_death(philo);
				return (0);
			}
		}
		pthread_mutex_unlock(philo->m_life);
		ft_msleep(2, NULL);
		pthread_mutex_lock(philo->m_tummy);
	}
	pthread_mutex_unlock(philo->m_tummy);
	return (0);
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(philo->m_print);
	if (*philo->can_print)
	{
		printf("%5lu %3i has died\n", ft_getmsofday() - *philo->t0, philo->num);
		ft_msleep(philo->args[TT_DIE] + 10, NULL);
	}
	*philo->can_print = 0;
	pthread_mutex_unlock(philo->m_print);
}
