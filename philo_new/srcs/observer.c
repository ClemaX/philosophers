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

/* static bool	observe_death(t_philo *philo)
{
	t_time	time_die;
	bool	running;

	pthread_mutex_lock(&philo->lock);
	time_die = philo->time_die;
	//dprintf(2, "observer: philo %llu: time_die: %llu\n", philo->index + 1, time_die - g_table.time_start);
	pthread_mutex_unlock(&philo->lock);
	sleep_until(time_die);
	//dprintf(2, "observer: philo %llu: time_now: %llu\n", philo->index + 1, time_millis() - g_table.time_start);
	pthread_mutex_lock(&philo->lock);
	if (!(running = time_die != philo->time_die))
	{
		pthread_mutex_lock(&g_table.lock_run);
		if (g_table.running)
		{
			pthread_mutex_lock(&g_table.lock_write);
			table_log(philo->index, MSG_DIED, sizeof(MSG_DIED) - 1);
			g_table.running = false;
		}
		pthread_mutex_unlock(&g_table.lock_run);
	}
	pthread_mutex_unlock(&philo->lock);
	return (running);
} */


#include <stdio.h>

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

void	*observer_thread(void *data)
{
	t_philo *const	philo = data;
	t_philo_state	state;
	pthread_t		tid;
	int				err;
	t_time			now;
	bool			running = true;

	if ((err = pthread_create(&tid, NULL, &philo_thread, philo)))
	{
		table_perror("observer: pthread_create", err);
		return (data);
	}
	while (running)
	{
		pthread_mutex_lock(&g_table.lock_run);
		if ((running = g_table.running))
		{
			now = time_millis();
			pthread_mutex_lock(&philo->lock_state);
			if (now >= philo->time_starve)
			{
				observer_log(now, philo, STARVED);
				running = false;
				g_table.running = false;
			}
			pthread_mutex_unlock(&g_table.lock_run);
			if (running && philo->state_changed)
			{
				philo->state_changed = false;
				state = philo->state;
				pthread_mutex_unlock(&philo->lock_state);
				if (state == EATING)
					observer_log(now, philo, TAKING_FORK);
				observer_log(now, philo, state);
			}
			else
				pthread_mutex_unlock(&philo->lock_state);
		}
		else
			pthread_mutex_unlock(&g_table.lock_run);
	}
	if ((err = pthread_join(tid, &data)))
		table_perror("observer: pthread_join", err);
	return (NULL);
}
