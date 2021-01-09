/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 04:25:45 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 14:25:39 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <table.h>

bool	table_start(t_philo *philos)
{
	t_uint	i;

	i = 0;
	if (table_set(philos))
	{
		g_table.time_start = time_millis();
		while (i < g_table.seats && philo_start(&philos[i++]))
			usleep(100);
	}
	return (i == g_table.seats);
}

void	table_stop(void)
{
	t_uint	i;

	i = 0;
	while (i < g_table.seats && sem_post(g_table.count_satisfied) != -1)
		i++;
	if (i != g_table.seats)
		table_perror("table: sem_post", errno);
}

bool	table_running(void)
{
	sem_wait(g_table.lock_run);
	sem_post(g_table.lock_run);
	return (true);
}

bool	table_take_fork(t_philo *philo)
{
	bool	running;

	sem_wait(g_table.count_forks);
	if ((running = table_running()))
		table_log(philo, "has taken a fork");
	else
		sem_post(g_table.count_forks);
	return (running);
}

void	table_drop_fork(void)
{
	sem_post(g_table.count_forks);
}
