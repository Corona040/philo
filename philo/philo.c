/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:52:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/17 15:59:02 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int				i;
	static size_t	args[6];
	t_philo			*philos;
	static t_state	state;

	if (check_args(argc, argv))
		return (0);
	if (init_args(argc, argv, args))
		return (0);
	if (lone_philo(&state, args))
		return (0);
	if (allocate_n_set(&philos, &state, args))
	{
		malloc_fail(philos, &state);
		return (0);
	}
	pthread_mutex_lock(state.m_sync);
	set_philos(philos, &state, args);
	start_dinner(philos, &state, args);
	i = 0;
	while (i < (int)args[N_PHILO])
		pthread_join(philos[i++].thread, NULL);
	destroy_n_free(philos, &state, args);
	return (0);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (1);
	if (argc == 6 && ft_atoi(argv[5]) <= 0)
		return (1);
	i = 0;
	while (i++ < argc - 1)
	{
		if (i != 5 && ft_atoi(argv[i]) <= 0)
			return (1);
		j = 0;
		while (argv[i][j])
			if (!ft_isdigit(argv[i][j++]))
				return (1);
	}
	return (0);
}

int	init_args(int argc, char **argv, size_t *args)
{
	int	i;

	i = 0;
	while (i++ < argc - 1)
		args[i - 1] = ft_atoi(argv[i]);
	if (args[TT_EAT] > args[TT_SLEEP])
		args[TT_THINK] = args[TT_EAT] - args[TT_SLEEP] + 1;
	else
		args[TT_THINK] = 1;
	return (0);
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
