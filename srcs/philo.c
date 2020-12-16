#include <philo.h>

bool	philo_log(t_philo *philo, const char *message)
{
	const t_time	now = clock_millis() - philo->table->time_start;
	int				err;

	if (!(err = pthread_mutex_lock(&philo->table->write_lock)))
	{
		putui(STDOUT_FILENO, now);
		write(STDOUT_FILENO, " ", 1);
		putui(STDOUT_FILENO, philo->index);
		write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, message, ft_strlen(message));
		write(STDOUT_FILENO, "\n", 1);
		if (!(err = pthread_mutex_unlock(&philo->table->write_lock)))
			return (true);
	}
	return (false);
}


void	philo_eat(t_philo *philo)
{
	(void)philo;

}

void	philo_sleep(t_philo *philo)
{
	(void)philo;

}

void	philo_think(t_philo *philo)
{
	(void)philo;

}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;

	philo_log(philo, "Hello from philo!");
	philo_eat(philo);
	philo_sleep(philo);
	philo_think(philo);
	return (NULL);
}
