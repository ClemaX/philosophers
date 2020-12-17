#include <philo.h>

bool	philo_log(t_philo *philo, const char *message)
{
	const t_time	now = clock_millis() - philo->table->time_start;
	int				err;

	if (!(err = pthread_mutex_lock(&philo->table->write_lock)))
	{
		putui(STDOUT_FILENO, now);
		write(STDOUT_FILENO, " ", 1);
		putui(STDOUT_FILENO, philo->index + 1);
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
		if (!(err = pthread_mutex_unlock(&philo->table->write_lock)))
			return (true);
	}
	return (false);
}

// TODO: Fix deadlock when philos each take one fork
void	philo_eat(t_philo *philo)
{
	const size_t    forks[2] = {
		philo->index,
		(philo->index + 1 == philo->table->seats) ? 0 : philo->index + 1
	};
	int				err;

	if (!(err = pthread_mutex_lock(&philo->table->forks[forks[0]])))
	{
		philo_log(philo, "has taken a fork");
		if (!(err = pthread_mutex_lock(&philo->table->forks[forks[1]])))
		{
			philo_log(philo, "has taken a fork");
			philo_log(philo, "is eating");
			usleep(philo->table->time_to_eat * 1000);
			pthread_mutex_unlock(&philo->table->forks[forks[1]]);
		}
		pthread_mutex_unlock(&philo->table->forks[forks[0]]);
	}
	if (err)
		dprintf(STDERR_FILENO, "pthread_mutex_lock: (%d)%s, forks: %zu, %zu\n",
			err, strerror(err), forks[0], forks[1]);
}

void	philo_sleep(t_philo *philo)
{
	philo_log(philo, "is sleeping");
	usleep(philo->table->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	philo_log(philo, "is thinking");
	usleep(10 * 1000);
}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;
	uint64_t		times_ate;

	times_ate = 0;
	while (times_ate++ < philo->table->appetite)
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}
