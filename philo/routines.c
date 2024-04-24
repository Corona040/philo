/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 10:54:47 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/24 10:31:09 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_forks(t_philo *philo)
{
	pthread_t	thread_lfork;
	pthread_t	thread_rfork;

	pthread_create(&thread_lfork, NULL, get_lfork, (void *)(philo));
	pthread_create(&thread_rfork, NULL, get_rfork, (void *)(philo));
	pthread_join(thread_lfork, NULL);
	pthread_join(thread_rfork, NULL);
	return (0);
}

void	*get_lfork(void *arg)
{
	t_philo	*philo;
	size_t	ms;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->lfork);
	ms = ft_getmsofday();
	printf("%5i %3i has taken a fork\n", (int)ms - (int)philo->t0, (int)philo->num);
	return (0);
}

void	*get_rfork(void *arg)
{
	t_philo	*philo;
	size_t	ms;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->rfork);
	ms = ft_getmsofday();
	printf("%5i %3i has taken a fork\n", (int)ms - (int)philo->t0, (int)philo->num);
	return (0);
}

int	p_sleep(t_philo *philo)
{
	size_t	ms;

	ms = ft_getmsofday();
	printf("%5i %3i is sleeping\n", (int)ms - (int)philo->t0, (int)philo->num);
	ft_msleep(philo->args->tt_sleep);
	ms = ft_getmsofday();
	printf("%5i %3i is thinking\n", (int)ms - (int)philo->t0, (int)philo->num);
	return (0);
}

int	p_eat(t_philo *philo)
{
	size_t	ms;

	ms = ft_getmsofday();
	philo->die_time = ms - philo->t0 + philo->args->tt_die;
	printf("%5i %3i is eating\n", (int)ms - (int)philo->t0, (int)philo->num);
	ft_msleep(philo->args->tt_eat);
	pthread_mutex_unlock(philo->lfork);
	pthread_mutex_unlock(philo->rfork);
	philo->eat_count++;
	return (0);
}
