/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 02:05:35 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 16:03:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static t_time	philo_take_forks(t_philo *philo)
{
	t_time	time_now;

	sem_wait(g_table.fork_count);
	if ((time_now = table_log(philo, "has taken a fork")))
	{
		sem_wait(g_table.fork_count);
		if ((time_now = table_log(philo, "has taken a fork")))
			return (time_now);
		sem_post(g_table.fork_count);
	}
	sem_post(g_table.fork_count);
	return (time_now);
}

static void		philo_drop_forks(void)
{
	sem_post(g_table.fork_count);
	sem_post(g_table.fork_count);
}

t_time			philo_eat(t_philo *philo)
{
	t_time	time_now;

	if ((time_now = philo_take_forks(philo))
	&& (time_now = table_log(philo, "is eating")))
	{
		sem_wait(philo->lock);
		philo->time_die = time_now + g_table.time_to_die;
		sem_post(philo->lock);
		sleep_until(time_now + g_table.time_to_eat);
		philo_drop_forks();
		if (g_table.appetite && philo->times_ate < g_table.appetite
		&& ++philo->times_ate == g_table.appetite)
		{
			sem_wait(g_table.lock_run);
			if (++g_table.satisfied == g_table.seats)
				g_table.running = false;
			sem_post(g_table.lock_run);
		}
	}
	return (time_now);
}

t_time			philo_sleep(t_philo *philo)
{
	const t_time	time_now = table_log(philo, "is sleeping");

	if (time_now)
		sleep_until(time_now + g_table.time_to_sleep);
	return (time_now);
}

t_time			philo_think(t_philo *philo)
{
	return (table_log(philo, "is thinking"));
}
