/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/30 13:17:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

bool			philo_set(t_philo *philo, t_uint index)
{
	int	err;

	if (!(err = pthread_mutex_init(&philo->lock_time_starve, NULL)))
	{
		ft_strcpy(philo->log_buffer, MSG_TEMPLATE);
		philo->forks[0] = index;
		philo->forks[1] = (index + 1) % g_table.seats;
		philo->index = index;
		philo->times_ate = 0;
		philo->time_starve = 0;
	}
	return (!err);
}

static t_time	philo_take_forks(t_philo *philo)
{
	t_time	time_now;

	pthread_mutex_lock(&g_table.forks[philo->forks[0]]);
	if ((time_now = philo_log(philo, TAKING_FORK)))
	{
		pthread_mutex_lock(&g_table.forks[philo->forks[1]]);
		if ((time_now = philo_log(philo, TAKING_FORK)))
			return (time_now);
	}
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	return (time_now);
}

static t_time	philo_eat(t_philo *philo)
{
	t_time	time_now;

	if ((time_now = philo_take_forks(philo))
	&& (time_now = philo_log(philo, EATING)))
	{
		pthread_mutex_lock(&philo->lock_time_starve);
		philo->time_starve = time_now + g_table.time_to_starve;
		pthread_mutex_unlock(&philo->lock_time_starve);
		sleep_until(time_now + g_table.time_to_eat);
		if (g_table.appetite && philo->times_ate < g_table.appetite
		&& ++philo->times_ate == g_table.appetite)
		{
			pthread_mutex_lock(&g_table.lock_run);
			if (++g_table.satisfied == g_table.seats)
			{
				g_table.running = false;
				time_now = 0;
			}
			pthread_mutex_unlock(&g_table.lock_run);
		}
		pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
		pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	}
	return (time_now);
}

static t_time	philo_sleep(t_philo *philo)
{
	const t_time	time_now = philo_log(philo, SLEEPING);

	if (time_now)
		sleep_until(time_now + g_table.time_to_sleep);
	return (time_now);
}

void			*philo_thread(void *data)
{
	t_philo *const	philo = data;

	while (philo_eat(philo)
	&& philo_sleep(philo)
	&& philo_log(philo, THINKING))
		;
	return (data);
}
