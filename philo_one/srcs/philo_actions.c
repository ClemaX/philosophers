#include <philo.h>

static bool	philo_take_forks(t_philo *philo)
{
	if (table_running())
	{
		pthread_mutex_lock(&g_table.forks[philo->forks[0]]);
		if (table_running())
		{
			table_log(philo, "has taken a fork");
			pthread_mutex_lock(&g_table.forks[philo->forks[1]]);
			if (table_running())
			{
				table_log(philo, "has taken a fork");
				return (true);
			}
			pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
		}
		pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
	}
	return (false);
}

static void	philo_drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&g_table.forks[philo->forks[1]]);
	pthread_mutex_unlock(&g_table.forks[philo->forks[0]]);
}

bool		philo_eat(t_philo *philo)
{
	bool	running;

	if ((running = philo_take_forks(philo)))
	{
		pthread_mutex_lock(&philo->lock);
		philo->time_die = time_millis() + g_table.time_to_die;
		pthread_mutex_unlock(&philo->lock);
		running = philo_sleep(philo, g_table.time_to_eat, "is eating");
		philo_drop_forks(philo);
		if (running && g_table.appetite
		&& philo->times_ate < g_table.appetite
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
	return (running);
}

bool		philo_sleep(t_philo *philo, t_time duration, const char *message)
{
	bool	running;
	t_time	now;
	t_time	time_die;

	if ((running = table_running()))
	{
		pthread_mutex_lock(&philo->lock);
		time_die = philo->time_die;
		pthread_mutex_unlock(&philo->lock);
		now = table_log(philo, message);
		if (now + duration > time_die)
			duration = time_die - now;
		usleep(duration * 1000);
	}
	return (running);
}

bool		philo_think(t_philo *philo)
{
	bool	running;

	if ((running = table_running()))
		table_log(philo, "is thinking");
	return (running);
}
