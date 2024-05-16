/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 12:42:17 by ecorona-         ###   ########.fr       */
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

# define N_PHILO 0
# define TT_DIE 1
# define TT_EAT 2
# define TT_SLEEP 3
# define N_EAT 4

# define FORK 0
# define EAT 1
# define SLEEP 2
# define THINK 3

typedef struct s_philo
{
	pthread_t		thread;
	pthread_t		life_support;
	int				num;
	size_t			*args;
	pthread_mutex_t	*m_lfork;
	pthread_mutex_t	*m_rfork;
	pthread_mutex_t	*m_print;
	pthread_mutex_t	*m_tummy;
	pthread_mutex_t	*m_life;
	pthread_mutex_t	*m_sync;
	int				eat_count;
	size_t			*t0;
	size_t			die_time;
	int				*can_print;
	int				*life;
	int				*update_ms;
}	t_philo;

typedef struct s_state
{
	pthread_mutex_t	*m_forks;
	pthread_mutex_t	*m_tummies;
	pthread_mutex_t	*m_print;
	pthread_mutex_t	*m_life;
	pthread_mutex_t	*m_sync;
	size_t			*ms;
	int				*update_ms;
	int				*can_print;
	int				*life;
}	t_state;

size_t	ft_getmsofday(void);
int		ft_msleep(size_t ms, t_philo *philo);
void	*routine(void *arg);
void	print_action(t_philo *philo, int print_code);
int		monitor(t_philo *philos, pthread_mutex_t *m_monitor, pthread_mutex_t *m_print, pthread_mutex_t *m_life, int *can_print, int *life);
void	*life_support(void *arg);

#endif
