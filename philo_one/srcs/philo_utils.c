/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/30 13:17:34 by chamada           #+#    #+#             */
/*   Updated: 2021/01/30 13:17:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

t_time	philo_log(t_philo *philo, t_philo_state state)
{
	const char		*message = g_state_msgs[state];
	size_t			index;
	bool			running;
	t_time			time_now;

	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&philo->log_buffer[index], philo->index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (*message)
		philo->log_buffer[index++] = *message++;
	philo->log_buffer[index++] = '\n';
	pthread_mutex_lock(&g_table.lock_run);
	if ((running = g_table.running))
	{
		time_now = time_millis();
		strputui(philo->log_buffer, time_now - g_table.time_start, MSG_TSLEN);
		write(STDOUT_FILENO, philo->log_buffer, index);
	}
	pthread_mutex_unlock(&g_table.lock_run);
	return (running ? time_now : 0);
}
