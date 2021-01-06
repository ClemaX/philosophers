#include <table.h>

void	table_show_usage(const char *name)
{
	write(STDERR_FILENO, MSG_EUSAGE_PREFIX, sizeof(MSG_EUSAGE_PREFIX) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, MSG_EUSAGE_SUFFIX, sizeof(MSG_EUSAGE_SUFFIX) - 1);
}

t_time	table_log(t_philo *philo, const char *message)
{
	const t_time	now = time_millis() - g_table.time_start;

	sem_wait(g_table.lock_write);
	putui(STDOUT_FILENO, now, 8);
	write(STDOUT_FILENO, " ", 1);
	putui(STDOUT_FILENO, philo->index + 1, 2);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
	sem_post(g_table.lock_write);
	return (now);
}

void	table_perror(const char *msg, int err)
{
	const char *const	err_msg = ft_strerror(err);

	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, ": ", 2);
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	else
	{
		write(STDERR_FILENO, "Unknown error: ", 15);
		putui(STDERR_FILENO, (t_uint)err, 0);
	}
	write(STDERR_FILENO, "\n", 1);
}