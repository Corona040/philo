/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:46:25 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/17 15:43:33 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_fork(t_philo *philo)
{
	size_t	ms;

	pthread_mutex_lock(philo->m_print);
	ms = ft_getmsofday();
	if (*philo->can_print)
		printf("%5lu %3i has taken a fork\n", ms - *philo->t0, philo->num);
	pthread_mutex_unlock(philo->m_print);
}

void	print_eat(t_philo *philo)
{
	size_t	ms;

	pthread_mutex_lock(philo->m_print);
	ms = ft_getmsofday();
	if (*philo->can_print)
		printf("%5lu %3i is eating\n", ms - *philo->t0, philo->num);
	pthread_mutex_unlock(philo->m_print);
}

void	print_sleep(t_philo *philo)
{
	size_t	ms;

	pthread_mutex_lock(philo->m_print);
	ms = ft_getmsofday();
	if (*philo->can_print)
		printf("%5lu %3i is sleeping\n", ms - *philo->t0, philo->num);
	pthread_mutex_unlock(philo->m_print);
}

void	print_think(t_philo *philo)
{
	size_t	ms;

	pthread_mutex_lock(philo->m_print);
	ms = ft_getmsofday();
	if (*philo->can_print)
		printf("%5lu %3i is thinking\n", ms - *philo->t0, philo->num);
	pthread_mutex_unlock(philo->m_print);
}
