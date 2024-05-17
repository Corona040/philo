/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:27:26 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/17 15:43:20 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

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
# define TT_THINK 5

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
	int				eat_c;
	size_t			*t0;
	size_t			die_time;
	int				*can_print;
	int				is_alive;
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

/* ************************************************************************** */
//		ft_atoi.c
int		ft_isdigit(int c);
int		ft_atoi(const char *nptr);
/* ************************************************************************** */
//		philo.c
int		check_args(int argc, char **argv);
int		init_args(int argc, char **argv, size_t *args);
void	start_dinner(t_philo *philos, t_state *state, size_t *args);
/* ************************************************************************** */
//		philo2.c
int		lone_philo(t_state *state, size_t *args);
int		allocate_n_set(t_philo **philos, t_state *state, size_t *args);
void	set_philos(t_philo *philos, t_state *state, size_t *args);
void	destroy_n_free(t_philo *philos, t_state *state, size_t *args);
void	malloc_fail(t_philo *philos, t_state *state);
/* ************************************************************************** */
//		time.c
size_t	ft_getmsofday(void);
int		ft_msleep(size_t ms, t_philo *philo);
/* ************************************************************************** */
//		routine.c
void	*routine(void *arg);
void	philo_sync(t_philo *philo);
void	get_forks(t_philo *philo);
int		eat_sleep(t_philo *philo);
int		eating(t_philo *philo);
/* ************************************************************************** */
//		monitor.c
void	*life_support(void *arg);
void	print_death(t_philo *philo);
/* ************************************************************************** */
//		print_action.c
void	print_action(t_philo *philo, void (*print_act)(t_philo *));
void	print_fork(t_philo *philo);
void	print_eat(t_philo *philo);
void	print_sleep(t_philo *philo);
void	print_think(t_philo *philo);

#endif
