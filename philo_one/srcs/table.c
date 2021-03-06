/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/30 13:17:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <table.h>

bool		table_new(t_philo **philos, int ac, const char **av)
{
	g_table.appetite = 0;
	if ((ac == 5 || (ac == 6 && (g_table.appetite = atoui(av[5]))))
	&& (g_table.seats = atoui(av[1]))
	&& g_table.seats > 1 && g_table.seats < 300
	&& (g_table.time_to_starve = atoui(av[2]))
	&& (g_table.time_to_eat = atoui(av[3]))
	&& (g_table.time_to_sleep = atoui(av[4])))
	{
		if ((*philos = malloc(sizeof(**philos) * g_table.seats)))
		{
			g_table.forks = malloc(sizeof(*g_table.forks) * g_table.seats);
			if (g_table.forks)
				return (true);
			free(*philos);
		}
		table_perror("table: malloc", errno);
	}
	else
		table_show_usage(av[0]);
	return (false);
}

void		table_del(t_philo **philos)
{
	t_uint	i;

	i = 0;
	while (i < g_table.seats)
	{
		pthread_mutex_destroy(&g_table.forks[i]);
		pthread_mutex_destroy(&(*philos)[i++].lock_time_starve);
	}
	pthread_mutex_destroy(&g_table.lock_run);
	free(g_table.forks);
	free(*philos);
	g_table.forks = NULL;
	*philos = NULL;
}

static bool	table_set(t_philo *philos)
{
	t_uint	i;
	int		err;

	i = 0;
	if (!(err = pthread_mutex_init(&g_table.lock_run, NULL)))
	{
		while (i < g_table.seats
		&& !(err = pthread_mutex_init(&g_table.forks[i], NULL))
		&& philo_set(&philos[i], i))
			i++;
		if (i == g_table.seats)
		{
			g_table.satisfied = 0;
			return (true);
		}
	}
	g_table.seats = i;
	table_perror("table: pthread_mutex_init", err);
	return (false);
}

bool		table_start(t_philo *philos)
{
	struct timeval	tv_now;
	t_uint			i;
	int				err;

	if (!table_set(philos))
		return (false);
	i = 0;
	err = 0;
	g_table.running = true;
	g_table.time_start = time_millis();
	while (!err && i < g_table.seats)
	{
		gettimeofday(&tv_now, NULL);
		philos[i].time_starve = time_millis() + g_table.time_to_starve;
		err = pthread_create(&philos[i].tid_observer, NULL,
			&observer_thread, &philos[i]);
		i++;
		usleep(1000);
	}
	if (i != g_table.seats)
		table_perror("table: pthread_create", err);
	return (!err);
}

bool		table_join(t_philo *philos)
{
	t_uint	i;
	int		err;

	i = 0;
	err = 0;
	while (i < g_table.seats
	&& !(err = pthread_join(philos[i].tid_observer, NULL)))
		i++;
	if (i != g_table.seats)
		table_perror("table: pthread_join", err);
	return (!err);
}
