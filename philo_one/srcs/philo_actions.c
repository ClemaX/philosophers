/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 23:55:53 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 16:03:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static bool	philo_take_forks(t_philo *philo)
{
	t_time	time_ate;

	pthread_mutex_lock(&g_table.forks[philo->forks[0]]);
	if (table_running_log(philo, MSG_TAKE_FORK, sizeof(MSG_TAKE_FORK) - 1))
	{
		pthread_mutex_lock(&g_table.forks[philo->forks[1]]);
		time_ate = time_millis();
		pthread_mutex_lock(&philo->lock);
		philo->time_die = time_ate + g_table.time_to_die;
		pthread_mutex_unlock(&philo->lock);
		if (table_running_log(philo, MSG_TAKE_FORK, sizeof(MSG_TAKE_FORK) - 1)
		&& philo_sleep(philo, time_ate + g_table.time_to_eat,
			MSG_EATING, sizeof(MSG_EATING) - 1))
			return (true);
		pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
	}
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	return (false);
}

static void	philo_drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
}

bool		philo_eat(t_philo *philo)
{
	bool	running;

	if ((running = (philo_take_forks(philo))))
	{
		philo_drop_forks(philo);
		if (g_table.appetite
		&& philo->times_ate < g_table.appetite
		&& ++philo->times_ate == g_table.appetite)
		{
			pthread_mutex_lock(&g_table.lock_run);
			if (++g_table.satisfied == g_table.seats)
			{
				g_table.running = false;
				running = false;
			}
			pthread_mutex_unlock(&g_table.lock_run);
		}
	}
	return (running);
}

bool		philo_sleep(t_philo *philo, t_time time_wake, const char *message,
	size_t length)
{
	bool	running;
	t_time	time_die;
	t_time	duration;

	pthread_mutex_lock(&philo->lock);
	time_die = philo->time_die;
	pthread_mutex_unlock(&philo->lock);
	if ((running = table_running_log(philo, message, length)))
	{
		duration = ((time_wake < time_die) ? time_wake : time_die)
			- time_millis();
		usleep(duration * 1000);
	}
	return (running);
}

bool		philo_think(t_philo *philo)
{
	return (table_running_log(philo, MSG_THINKING, sizeof(MSG_THINKING) - 1));
}
