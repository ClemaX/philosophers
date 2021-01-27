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

void	observer_log(t_time now, t_philo *philo, t_philo_state state)
{
	const char		*message = g_state_msgs[state];
	size_t			index;

	strputui(philo->log_buffer, now - g_table.time_start, MSG_TSLEN);
	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&philo->log_buffer[index], philo->index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (*message)
		philo->log_buffer[index++] = *message++;
	philo->log_buffer[index++] = '\n';
	write(STDOUT_FILENO, philo->log_buffer, index);
}

static void	observe_death(t_philo *philo)
{
	bool	running;
	t_time	time_starve;
	t_time	now;

	pthread_mutex_lock(&g_table.lock_run);
	running = g_table.running;
	pthread_mutex_unlock(&g_table.lock_run);
	while (running)
	{
		pthread_mutex_lock(&philo->lock_time_starve);
		time_starve = philo->time_starve;
		pthread_mutex_unlock(&philo->lock_time_starve);
		if ((now = time_millis()) >= time_starve)
		{
			pthread_mutex_lock(&g_table.lock_run);
			if (g_table.running)
			{
				g_table.running = false;
				observer_log(now, philo, STARVED);
			}
			pthread_mutex_unlock(&g_table.lock_run);
			running = false;
		}
		else
		{
			usleep(time_starve - now);
			pthread_mutex_lock(&g_table.lock_run);
			running = g_table.running;
			pthread_mutex_unlock(&g_table.lock_run);
		}
	}
}

void	*observer_thread(void *data)
{
	t_philo *const	philo = data;
	pthread_t		tid;
	int				err;

	if ((err = pthread_create(&tid, NULL, &philo_thread, philo)))
	{
		table_perror("observer: pthread_create", err);
		return (data);
	}
	observe_death(philo);
	if ((err = pthread_join(tid, &data)))
		table_perror("observer: pthread_join", err);
	return (NULL);
}
