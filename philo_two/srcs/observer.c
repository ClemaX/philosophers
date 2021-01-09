/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 02:05:35 by chamada           #+#    #+#             */
/*   Updated: 2021/01/02 15:00:39 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <observer.h>

static bool	observe_death(t_philo *philo)
{
	t_time	time_die;
	bool	running;

	sem_wait(philo->lock);
	time_die = philo->time_die;
	sem_post(philo->lock);
	sleep_until(time_die);
	sem_wait(philo->lock);
	if (!(running = time_die != philo->time_die))
	{
		sem_wait(g_table.lock_run);
		if (g_table.running)
		{
			table_log(philo, "died");
			g_table.running = false;
		}
		sem_post(g_table.lock_run);
	}
	sem_post(philo->lock);
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
