#include <philo.h>

bool	philo_take_forks(t_philo *philo)
{
	bool	running;

	running = true;
	while (running && table_running(philo->table))
	{
		sem_wait(philo->table->lock_fork_count);
		if ((running = table_running(philo->table)) && philo->table->fork_count >= 2)
		{
			philo->table->fork_count -= 2;
			philo_log(philo, "has taken a fork");
			philo_log(philo, "has taken a fork");
			sem_post(philo->table->lock_fork_count);
			return (true);
		}
		sem_post(philo->table->lock_fork_count);
	}
	return (false);
}

bool	philo_eat(t_philo *philo)
{
	bool	running;

	if ((running = philo_take_forks(philo)))
	{
		sem_wait(philo->lock);
		if ((running = table_running(philo->table)))
		{
			philo_log(philo, "is eating");
			if (philo->table->appetite)
				running = ++philo->times_ate < philo->table->appetite;
			philo->time_die = clock_millis() + philo->table->time_to_die;
			sem_post(philo->lock);
			usleep(philo->table->time_to_eat * 1000);
		}
		else
			sem_post(philo->lock);
	}
	return (running);
}

bool	philo_sleep(t_philo *philo)
{
	t_time	now;

	if (table_running(philo->table))
	{

		// TODO: Remove this
		now = clock_millis();
		if ((now + philo->table->time_to_sleep) < philo->time_die)
		{
			philo_log(philo, "is sleeping");
			usleep(philo->table->time_to_sleep * 1000);
			return (true);
		}
	}
	return (false);
}

bool	philo_think(t_philo *philo)
{
	bool	running;

	if ((running = table_running(philo->table)))
		philo_log(philo, "is thinking");
	return (running);
}
