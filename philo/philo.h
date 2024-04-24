/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                         /      \   /      \      */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/04/24 15:31:10 by eco                 \__/   \__/          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// memset
# include <string.h>
// printf
# include <stdio.h>
// malloc free
# include <stdlib.h>
// write usleep
# include <unistd.h>
// gettimeofday
# include <sys/time.h>
// pthread_
# include <pthread.h>

// enum e_state
// {
// 	sleep,
// 	eat,
// 	die
// };

typedef struct s_args
{
	size_t	n_philo;
	size_t	tt_die;
	size_t	tt_eat;
	size_t	tt_sleep;
	size_t	n_eat;
}	t_args;

typedef struct s_philo
{
	pthread_t		thread_id;
	size_t			num;
	t_args			*args;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	size_t			t0;
	int				eat_count;
	// int				is_alive;
	size_t			die_time;
}	t_philo;

size_t	ft_getmsofday(void);
int		ft_msleep(size_t ms);
void	*routine(void *arg);
int		get_forks(t_philo *philo);
void	*get_lfork(void *arg);
void	*get_rfork(void *arg);
int		p_sleep(t_philo *philo);
int		p_eat(t_philo *philo);
int		has_died(t_philo *philos);

#endif
