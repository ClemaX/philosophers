#include <philo.h>

bool	philo_log(t_philo *philo, const char *message)
{
	const t_time	now = clock_millis() - g_table.time_start;
	int				err;

	if (!(err = pthread_mutex_lock(&g_table.lock_write)))
	{
		putui(STDOUT_FILENO, now);
		write(STDOUT_FILENO, " ", 1);
		putui(STDOUT_FILENO, philo->index + 1);
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
		if (!(err = pthread_mutex_unlock(&g_table.lock_write)))
			return (true);
	}
	return (false);
}

// TODO: Forks should be initialized at the start since these are runtime-constants
// TODO: Maybe reject single dining philosopher
bool	philo_eat(t_philo *philo)
{
	const uint64_t	wrapped_fork = (philo->index + 1) % g_table.seats;
	const uint64_t	forks[2] = {
		wrapped_fork == 0 ? wrapped_fork : philo->index,
		wrapped_fork == 0 ? philo->index : wrapped_fork,
	};
	bool			running;

	if ((running = table_running()))
	{
		pthread_mutex_lock(&g_table.forks[forks[0]]);
		if ((running = table_running()))
		{
			philo_log(philo, "has taken a fork");
			if ((running = forks[0] != forks[1]))
			{
				pthread_mutex_lock(&g_table.forks[forks[1]]);
				if ((running = table_running()))
				{
					philo_log(philo, "has taken a fork");
					philo_log(philo, "is eating");
					pthread_mutex_lock(&philo->lock);
					if (g_table.appetite)
						running = ++philo->times_ate < g_table.appetite;
					philo->time_die = clock_millis() + g_table.time_to_die;
					pthread_mutex_unlock(&philo->lock);
					usleep(g_table.time_to_eat * 1000);
				}
				pthread_mutex_unlock(&g_table.forks[forks[1]]);
			}
		}
		pthread_mutex_unlock(&g_table.forks[forks[0]]);
	}
	return (running);
}

bool	philo_sleep(t_philo *philo)
{
	t_time	now;

	if (table_running())
	{
		// FIXME
		now = clock_millis();
		philo_log(philo, "is sleeping");
		if ((now + g_table.time_to_sleep) < philo->time_die)
		{
			usleep(g_table.time_to_sleep * 1000);
			return (true);
		}
		else
			usleep((philo->time_die - now) * 1000);
	}
	return (false);
}

bool	philo_think(t_philo *philo)
{
	if (table_running())
	{
		philo_log(philo, "is thinking");
		//usleep(10 * 1000);
		return (true);
	}
	return (false);
}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;

	pthread_mutex_lock(&philo->lock);
	philo->times_ate = 0;
	philo->time_die = clock_millis() + g_table.time_to_die;
	pthread_mutex_unlock(&philo->lock);
	while (philo_eat(philo)
	&& philo_sleep(philo)
	&& philo_think(philo))
		;
	return (NULL);
}
