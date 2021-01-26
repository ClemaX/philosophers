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

void	table_log(t_uint philo_index, const char *message, size_t length)
{
	static char		template[] = MSG_TEMPLATE;
	const t_time	now = time_millis();
	size_t			index;

	strputui(template, now - g_table.time_start, MSG_TSLEN);
	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&template[index], philo_index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (length--)
		template[index++] = *message++;
	template[index++] = '\n';
	write(STDOUT_FILENO, template, index);
}

bool	table_running(void)
{
	bool	running;

	pthread_mutex_lock(&g_table.lock_run);
	running = g_table.running;
	pthread_mutex_unlock(&g_table.lock_run);
	return (running);
}

/* void	table_log_async(t_uint philo_index, const char *message, size_t length)
{
	static char		template[] = MSG_TEMPLATE;
	const t_time	now = time_millis();
	size_t			index;

	pthread_mutex_lock(&g_table.lock_write);
	strputui(template, now - g_table.time_start, MSG_TSLEN);
	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&template[index], philo_index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (length--)
		template[index++] = *message++;
	template[index++] = '\n';
	write(STDOUT_FILENO, template, index);
	pthread_mutex_unlock(&g_table.lock_write);
} */

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
