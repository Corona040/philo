/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecorona- <ecorona-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:52:56 by ecorona-          #+#    #+#             */
/*   Updated: 2024/05/16 20:56:13 by ecorona-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int				i;
	static size_t	args[6];
	t_philo			*philos;
	t_state			state;

	if (init_args(argc, argv, args))
		return (0);
	if (lone_philo(&state, args))
		return (0);
	allocate_n_set(&philos, &state, args);
	pthread_mutex_lock(state.m_sync);
	set_philos(philos, &state, args);
	start_dinner(philos, &state, args);
	i = 0;
	while (i < (int)args[N_PHILO])
		pthread_join(philos[i++].thread, NULL);
	destroy_n_free(philos, &state, args);
	return (0);
}

int	init_args(int argc, char **argv, size_t *args)
{
	int	i;

	if ((argc != 5 && argc != 6) || (argc == 6 && !ft_atoi(argv[5])))
		return (1);
	i = 0;
	while (i++ < argc - 1)
	{
		args[i - 1] = ft_atoi(argv[i]);
		if (i != 5 && args[i - 1] == 0)
			return (1);
	}
	if (args[TT_EAT] > args[TT_SLEEP])
		args[TT_THINK] = args[TT_EAT] - args[TT_SLEEP] + 1;
	else
		args[TT_THINK] = 1;
	return (0);
}
