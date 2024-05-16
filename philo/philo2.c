/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 20:42:33 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 20:55:56 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lone_philo(t_state *state, size_t *args)
{
	if (args[N_PHILO] == 1)
	{
		state->ms = malloc(sizeof(size_t));
		if (!state->ms)
			return (1);
		*state->ms = ft_getmsofday();
		printf("%5lu %3i has taken a fork\n", ft_getmsofday() - *state->ms, 1);
		ft_msleep(args[TT_DIE], NULL);
		printf("%5lu %3i has died\n", ft_getmsofday() - *state->ms, 1);
		free(state->ms);
		return (1);
	}
	return (0);
}

void	allocate_n_set(t_philo **philos, t_state *state, size_t *args)
{
	*philos = malloc(args[N_PHILO] * sizeof(t_philo));
	state->m_forks = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	state->m_tummies = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	state->m_life = malloc(args[N_PHILO] * sizeof(pthread_mutex_t));
	state->m_print = malloc(sizeof(pthread_mutex_t));
	state->m_sync = malloc(sizeof(pthread_mutex_t));
	state->ms = malloc(sizeof(size_t));
	state->can_print = malloc(sizeof(int));
	state->update_ms = malloc(sizeof(int));
	*state->can_print = 1;
	*state->update_ms = 1;
	pthread_mutex_init(state->m_print, NULL);
	pthread_mutex_init(state->m_sync, NULL);
}

void	set_philos(t_philo *philos, t_state *state, size_t *args)
{
	int	i;

	i = 0;
	while (i < (int)args[N_PHILO])
	{
		philos[i].num = i + 1;
		philos[i].args = args;
		philos[i].m_rfork = state->m_forks + i;
		philos[i].m_tummy = state->m_tummies + i;
		philos[i].m_life = state->m_life + i;
		philos[i].m_print = state->m_print;
		philos[i].m_sync = state->m_sync;
		philos[i].eat_count = 0;
		philos[i].is_alive = 1;
		philos[i].can_print = state->can_print;
		philos[i].update_ms = state->update_ms;
		if (i > 0)
			philos[i].m_lfork = state->m_forks + i - 1;
		pthread_mutex_init(state->m_forks + i, NULL);
		pthread_mutex_init(state->m_tummies + i, NULL);
		pthread_mutex_init(state->m_life + i, NULL);
		i++;
	}
	philos[0].m_lfork = state->m_forks + args[N_PHILO] - 1;
}

void	start_dinner(t_philo *philos, t_state *state, size_t *args)
{
	int	i;

	i = 0;
	while (i < (int)args[N_PHILO])
	{
		philos[i].t0 = state->ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
	pthread_mutex_unlock(state->m_sync);
	usleep(10000);
	i = 1;
	while (i < (int)args[N_PHILO])
	{
		philos[i].t0 = state->ms;
		philos[i].die_time = philos[i].args[TT_DIE];
		pthread_create(&philos[i].thread, NULL, routine, (void *)(&philos[i]));
		i += 2;
	}
}

void	destroy_n_free(t_philo *philos, t_state *state, size_t *args)
{
	int	i;

	i = 0;
	while (i < (int)args[N_PHILO])
	{
		pthread_mutex_destroy(state->m_forks + i);
		pthread_mutex_destroy(state->m_tummies + i);
		pthread_mutex_destroy(state->m_life + i);
		i++;
	}
	pthread_mutex_destroy(state->m_print);
	pthread_mutex_destroy(state->m_sync);
	free(philos);
	free(state->m_forks);
	free(state->m_tummies);
	free(state->m_print);
	free(state->m_life);
	free(state->can_print);
	free(state->ms);
	free(state->update_ms);
}
