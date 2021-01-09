/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 23:55:53 by chamada           #+#    #+#             */
/*   Updated: 2021/01/07 18:45:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <table.h>

void	table_show_usage(const char *name)
{
	write(STDERR_FILENO, MSG_EUSAGE_PREFIX, sizeof(MSG_EUSAGE_PREFIX) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, MSG_EUSAGE_ARGS, sizeof(MSG_EUSAGE_ARGS) - 1);
	write(STDERR_FILENO, MSG_EUSAGE_OPTS, sizeof(MSG_EUSAGE_OPTS) - 1);
	write(STDERR_FILENO, MSG_EUSAGE_SUFFIX, sizeof(MSG_EUSAGE_SUFFIX) - 1);
}

t_time	table_log(t_philo *philo, const char *message)
{
	const t_time	now = time_millis() - g_table.time_start;

	pthread_mutex_lock(&g_table.lock_write);
	putui(STDOUT_FILENO, now, FW_TIMESTAMP);
	write(STDOUT_FILENO, " ", 1);
	putui(STDOUT_FILENO, philo->index + 1, g_table.fw_index);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
	pthread_mutex_unlock(&g_table.lock_write);
	return (now);
}

void	table_perror(const char *msg, int err)
{
	const char *const	err_msg = ft_strerror(err);

	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, ": ", 2);
	if (err_msg)
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	else
	{
		write(STDERR_FILENO, "Unknown error: ", 15);
		putui(STDERR_FILENO, (t_uint)err, 0);
	}
	write(STDERR_FILENO, "\n", 1);
}

bool	table_running(void)
{
	bool	running;

	pthread_mutex_lock(&g_table.lock_run);
	running = g_table.running;
	pthread_mutex_unlock(&g_table.lock_run);
	return (running);
}
