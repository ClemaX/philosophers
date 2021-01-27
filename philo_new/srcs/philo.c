#include <philo.h>

bool	philo_set(t_philo *philo, t_uint index)
{
	const t_uint	wrapped_fork = (index + 1) % g_table.seats;
	int				err;

	if (!(err = pthread_mutex_init(&philo->lock_time_starve, NULL)))
	{
		ft_strcpy(philo->log_buffer, MSG_TEMPLATE);
		if (wrapped_fork != 0)
		{
			philo->forks[0] = index;
			philo->forks[1] = (index + 1) % (g_table.seats);
		}
		else
		{
			philo->forks[0] = (index + 1) % (g_table.seats);
			philo->forks[1] = index;
		}
		philo->index = index;
		// TODO: philo->times_ate = 0;
		philo->time_starve = 0;
		dprintf(2, "philo %llu using forks %llu and %llu!\n", index + 1, index, (index + 1) % (g_table.seats));
	}
	return (!err);
}

bool	philo_log(t_time now, t_philo *philo, t_philo_state state)
{
	const char		*message = g_state_msgs[state];
	size_t			index;
	bool			running;

	strputui(philo->log_buffer, now - g_table.time_start, MSG_TSLEN);
	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&philo->log_buffer[index], philo->index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (*message)
		philo->log_buffer[index++] = *message++;
	philo->log_buffer[index++] = '\n';
	pthread_mutex_lock(&g_table.lock_run);
	if ((running = g_table.running))
		write(STDOUT_FILENO, philo->log_buffer, index);
	pthread_mutex_unlock(&g_table.lock_run);
	return (running);
}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;
	t_time	now;
	bool	running;

	running = true;
	while (running)
	{
		pthread_mutex_lock(&g_table.forks[philo->forks[0]]);
		if ((running = philo_log(time_millis(), philo, TAKING_FORK)))
		{
			pthread_mutex_lock(&g_table.forks[philo->forks[1]]);
			if ((running = philo_log((now = time_millis()), philo, TAKING_FORK)
			&& philo_log(now, philo, EATING)))
			{
				pthread_mutex_lock(&philo->lock_time_starve);
				philo->time_starve = time_millis() + g_table.time_to_starve;
				pthread_mutex_unlock(&philo->lock_time_starve);
				usleep(g_table.time_to_eat * 1000);
			}
			pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
		}
		pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
		if ((running = philo_log(time_millis(), philo, SLEEPING)))
		{
			usleep(g_table.time_to_sleep * 1000);
			running = philo_log(time_millis(), philo, THINKING);
		}
	}
	return (data);
}
