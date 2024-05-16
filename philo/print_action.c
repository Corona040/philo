/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:46:25 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 12:46:32 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
