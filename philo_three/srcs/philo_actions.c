/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 18:03:08 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 16:03:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static bool	philo_take_forks(t_philo *philo)
{
	if (table_take_fork(philo))
	{
		if (table_take_fork(philo))
			return (true);
		table_drop_fork();
	}
	return (false);
}

static void	philo_drop_forks(void)
{
	table_drop_fork();
	table_drop_fork();
}

bool		philo_eat(t_philo *philo)
{
	t_time	now;
	bool	running;

	if ((running = philo_take_forks(philo)))
	{
		now = time_millis();
		sem_wait(philo->lock);
		philo->time_die = now + g_table.time_to_die;
		sem_post(philo->lock);
		table_log(philo, "is eating");
		sleep_until(now + g_table.time_to_eat);
		philo_drop_forks();
		if (running && g_table.appetite
		&& philo->times_ate < g_table.appetite
		&& ++philo->times_ate == g_table.appetite)
			sem_post(g_table.count_satisfied);
	}
	return (running);
}

bool		philo_sleep(t_philo *philo)
{
	const t_time	time_wake = time_millis() + g_table.time_to_sleep;
	bool			running;

	if ((running = table_running()))
	{
		table_log(philo, "is sleeping");
		sleep_until(time_wake);
	}
	return (running);
}

bool		philo_think(t_philo *philo)
{
	bool	running;

	if ((running = table_running()))
		table_log(philo, "is thinking");
	return (running);
}
