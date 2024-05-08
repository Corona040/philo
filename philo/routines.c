/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 10:54:47 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/29 14:57:18 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_forks(t_philo *philo)
{
	size_t		ms;

	if (philo->lfork)
	{
		ms = ft_getmsofday();
		pthread_mutex_lock(philo->lfork);
		print_fork(philo->can_print, philo->print, ms - philo->t0, philo->num);
	}
	else
		ft_msleep(philo->args->tt_die);
	if (philo->rfork)
	{
		ms = ft_getmsofday();
		pthread_mutex_lock(philo->rfork);
		print_fork(philo->can_print, philo->print, ms - philo->t0, philo->num);
	}
	else
		ft_msleep(philo->args->tt_die);
	return (0);
}

void	print_fork(int *can_print, pthread_mutex_t *print, int time, int num)
{
	pthread_mutex_lock(print);
	if (*can_print != 0)
		printf("%5i %3i has taken a fork\n", time, num);
	pthread_mutex_unlock(print);
}

int	p_sleep(t_philo *philo)
{
	size_t	ms;

	ms = ft_getmsofday();
	print_sleep(philo->can_print, philo->print, ms - philo->t0, philo->num);
	ft_msleep(philo->args->tt_sleep);
	ms = ft_getmsofday();
	print_think(philo->can_print, philo->print, ms - philo->t0, philo->num);
	return (0);
}

void	print_sleep(int *can_print, pthread_mutex_t *print, int time, int num)
{
	pthread_mutex_lock(print);
	if (*can_print != 0)
		printf("%5i %3i is sleeping\n", time, num);
	pthread_mutex_unlock(print);
}

void	print_think(int *can_print, pthread_mutex_t *print, int time, int num)
{
	pthread_mutex_lock(print);
	if (*can_print != 0)
		printf("%5i %3i is thinking\n", time, num);
	pthread_mutex_unlock(print);
}

int	p_eat(t_philo *philo)
{
	size_t	ms;

	ms = ft_getmsofday();
	pthread_mutex_lock(philo->monitor_mutex);
	philo->die_time = ms - philo->t0 + philo->args->tt_die;
	pthread_mutex_unlock(philo->monitor_mutex);
	print_eat(philo->can_print, philo->print, ms - philo->t0, philo->num);
	ft_msleep(philo->args->tt_eat);
	if (philo->lfork)
		pthread_mutex_unlock(philo->lfork);
	if (philo->rfork)
		pthread_mutex_unlock(philo->rfork);
	pthread_mutex_lock(philo->monitor_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(philo->monitor_mutex);
	return (0);
}

void	print_eat(int *can_print, pthread_mutex_t *print, int time, int num)
{
	pthread_mutex_lock(print);
	if (*can_print != 0)
		printf("%5i %3i is eating\n", time, num);
	pthread_mutex_unlock(print);
}
