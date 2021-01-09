/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 16:03:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <observer.h>

static bool	observe_death(t_philo *philo)
{
	t_time	time_die;
	bool	running;

	pthread_mutex_lock(&philo->lock);
	time_die = philo->time_die;
	pthread_mutex_unlock(&philo->lock);
	sleep_until(time_die);
	pthread_mutex_lock(&philo->lock);
	if (!(running = time_die != philo->time_die))
	{
		pthread_mutex_lock(&g_table.lock_run);
		if (g_table.running)
		{
			table_log(philo, "died");
			g_table.running = false;
		}
		pthread_mutex_unlock(&g_table.lock_run);
	}
	pthread_mutex_unlock(&philo->lock);
	return (running);
}

void		*observer_thread(void *data)
{
	t_philo *const	philo = data;

	while (observe_death(philo))
		;
	pthread_join(philo->tid, &data);
	return (data);
}
