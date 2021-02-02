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

static void	observer_log(t_philo *philo, const char *message)
{
	putui(STDOUT_FILENO, time_millis() - g_table.time_start, FW_TIMESTAMP);
	write(STDOUT_FILENO, " ", 1);
	putui(STDOUT_FILENO, philo->index + 1, g_table.fw_index);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
}

static void	observe_death(t_philo *philo)
{
	t_time	time_die;

	while (table_running())
	{
		sem_wait(philo->lock);
		time_die = philo->time_die;
		sem_post(philo->lock);
		if (time_millis() >= time_die)
		{
			sem_wait(g_table.lock_run);
			if (g_table.running)
			{
				observer_log(philo, "died");
				g_table.running = false;
			}
			sem_post(g_table.lock_run);
			break ;
		}
		else
			sleep_until(time_die);
	}
}

void		*observer_thread(void *data)
{
	t_philo *const	philo = data;

	observe_death(philo);
	pthread_join(philo->tid, &data);
	return (data);
}
