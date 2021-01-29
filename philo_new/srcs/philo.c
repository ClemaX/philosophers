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
			philo->forks[1] = wrapped_fork;
		}
		else
		{
			philo->forks[0] = wrapped_fork;
			philo->forks[1] = index;
		}
		philo->index = index;
		philo->times_ate = 0;
		philo->time_starve = 0;
	}
	return (!err);
}

bool	philo_log(t_philo *philo, t_philo_state state)
{
	const char		*message = g_state_msgs[state];
	size_t			index;
	bool			running;

	index = MSG_TSLEN + sizeof(MSG_DELIM) - 1;
	strputui(&philo->log_buffer[index], philo->index + 1, MSG_IDXLEN);
	index += MSG_IDXLEN + sizeof(MSG_DELIM) - 1;
	while (*message)
		philo->log_buffer[index++] = *message++;
	philo->log_buffer[index++] = '\n';
	pthread_mutex_lock(&g_table.lock_run);
	if ((running = g_table.running))
	{
		strputui(philo->log_buffer, time_millis() - g_table.time_start, MSG_TSLEN);
		write(STDOUT_FILENO, philo->log_buffer, index);
	}
	pthread_mutex_unlock(&g_table.lock_run);
	return (running);
}

bool	philo_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&g_table.forks[philo->forks[0]]);
	if (philo_log(philo, TAKING_FORK))
	{
		pthread_mutex_lock(&g_table.forks[philo->forks[1]]);
		if (philo_log(philo, TAKING_FORK))
			return (true);
	}
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	return (false);
}

void	philo_drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
}

bool	philo_eat(t_philo *philo)
{
	t_time			time_now;
	bool			running;

	if ((running = philo_take_forks(philo)))
	{
		time_now = time_millis();
		if ((running = philo_log(philo, EATING)))
		{
			pthread_mutex_lock(&philo->lock_time_starve);
			philo->time_starve = time_now + g_table.time_to_starve;
			pthread_mutex_unlock(&philo->lock_time_starve);
			sleep_until(time_now + g_table.time_to_eat);
			if (g_table.appetite && philo->times_ate < g_table.appetite
			&& ++philo->times_ate == g_table.appetite)
			{
				pthread_mutex_lock(&g_table.lock_run);
				if (++g_table.satisfied == g_table.seats)
				{
					g_table.running = false;
					running = false;
				}
				pthread_mutex_unlock(&g_table.lock_run);
			}
		}
		philo_drop_forks(philo);
	}
	return (running);
}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;
	t_time			time_now;
	bool			running;

	running = true;
	while (running && (running = philo_eat(philo)))
	{
		time_now = time_millis();
		if ((running = philo_log(philo, SLEEPING)))
		{
			sleep_until(time_now + g_table.time_to_sleep);
			running = philo_log(philo, THINKING);
		}
	}
	return (data);
}
