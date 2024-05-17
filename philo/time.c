/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:46:48 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/17 15:43:39 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			if (!philo->is_alive)
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
