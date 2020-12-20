#include <philo.h>

bool	philo_log(t_philo *philo, const char *message)
{
	const t_time	now = clock_millis() - philo->table->time_start;
	int				err;

	if (!(err = pthread_mutex_lock(&philo->table->lock_write)))
	{
		putui(STDOUT_FILENO, now);
		write(STDOUT_FILENO, " ", 1);
		putui(STDOUT_FILENO, philo->index + 1);
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
		if (!(err = pthread_mutex_unlock(&philo->table->lock_write)))
			return (true);
	}
	return (false);
}

bool	philo_alive(t_philo *philo)
{
	const bool	starved = clock_millis() >= philo->time_die;
	bool		running;

	pthread_mutex_lock(&philo->table->lock_run);
	if (philo->table->running && starved)
	{
		philo->table->running = false;
		philo_log(philo, "died");
	}
	running = philo->table->running;
	pthread_mutex_unlock(&philo->table->lock_run);
	return (running);
}

// TODO: Issue when philosophers starve, waiting for a fork
// TODO: Forks should be initialized at the start since these are runtime-constants
bool	philo_eat(t_philo *philo)
{
	const size_t	wrapped_fork = (philo->index + 1) % philo->table->seats;
	const size_t	forks[2] = {
		wrapped_fork == 0 ? wrapped_fork : philo->index,
		wrapped_fork == 0 ? philo->index : wrapped_fork,
	};
	bool			alive;

	if ((alive = philo_alive(philo)))
	{
		pthread_mutex_lock(&philo->table->forks[forks[0]]);
		if ((alive = philo_alive(philo)))
		{
			philo_log(philo, "has taken a fork");
			pthread_mutex_lock(&philo->table->forks[forks[1]]);
			if ((alive = philo_alive(philo)))
			{
				philo_log(philo, "has taken a fork");
				philo_log(philo, "is eating");
				philo->time_die = clock_millis() + philo->table->time_to_die;
				usleep(philo->table->time_to_eat * 1000);
			}
			pthread_mutex_unlock(&philo->table->forks[forks[1]]);
		}
		pthread_mutex_unlock(&philo->table->forks[forks[0]]);
	}
	return (alive);
}

bool	philo_sleep(t_philo *philo)
{
	t_time	now;

	if (philo_alive(philo))
	{
		now = clock_millis();
		if ((now + philo->table->time_to_sleep) < philo->time_die)
		{
			philo_log(philo, "is sleeping");
			usleep(philo->table->time_to_sleep * 1000);
			return (true);
		}
		else
		{
			usleep((philo->time_die - now )* 1000);
			if (philo_alive(philo))
				philo_log(philo, "has died");
		}
	}
	return (false);
}

bool	philo_think(t_philo *philo)
{
	if (philo_alive(philo))
	{
		philo_log(philo, "is thinking");
		//usleep(10 * 1000);
		return (true);
	}
	return (false);
}

// TODO: End simulation on philosopher death
void	*philo_thread(void *data)
{
	t_philo *const	philo = data;
	uint64_t		times_ate;

	philo->time_die = clock_millis() + philo->table->time_to_die;
	times_ate = 0;
	while (philo_eat(philo)
	&& (philo->table->appetite == 0 || times_ate++ < philo->table->appetite)
	&& philo_sleep(philo)
	&& philo_think(philo))
		;
	return (NULL);
}
