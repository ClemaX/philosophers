#include <table.h>

void	table_show_usage(const char *name)
{
	write(2, MSG_EUSAGE_PREFIX, sizeof(MSG_EUSAGE_PREFIX) - 1);
	write(2, name, ft_strlen(name));
	write(2, MSG_EUSAGE_SUFFIX, sizeof(MSG_EUSAGE_SUFFIX) - 1);
}

t_time	table_log(t_philo *philo, const char *message)
{
	const t_time	now = time_millis() - g_table.time_start;

	pthread_mutex_lock(&g_table.lock_write);
	putui(STDOUT_FILENO, now);
	write(STDOUT_FILENO, " ", 1);
	putui(STDOUT_FILENO, philo->index + 1);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
	pthread_mutex_unlock(&g_table.lock_write);
	return (now);
}

void	table_perror(const char *msg, int err)
{
	const char *const	err_msg = ft_strerror(err);

	write(2, msg, ft_strlen(msg));
	write(2, ": ", 2);
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	else
	{
		write(2, "Unknown error: ", 15);
		putui(2, (t_uint)err);
	}
	write(2, "\n", 1);
}
