/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:47:22 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 12:47:27 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
