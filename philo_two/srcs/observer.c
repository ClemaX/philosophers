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

static void	observe_death(t_philo *philo)
{
	t_time	now;
	t_time	time_die;
	bool	running;

	running = true;
	while (running)
	{
		sem_wait(philo->lock);
		time_die = philo->time_die;
		sem_post(philo->lock);
		if ((now = time_millis()) >= time_die)
		{
			sem_wait(g_table.lock_run);
			if (g_table.running)
			{
				table_log(philo, "died");
				g_table.running = false;
			}
			sem_post(g_table.lock_run);
			running = false;
		}
		else
		{
			sleep_until(time_die);
			sem_wait(g_table.lock_run);
			running = g_table.running;
			sem_post(g_table.lock_run);
		}
	}
}

void		*observer_thread(void *data)
{
	t_philo *const	philo = data;

	observe_death(philo);
	pthread_join(philo->tid, &data);
	return (data);
}
