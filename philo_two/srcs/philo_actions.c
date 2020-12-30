#include <philo.h>

// TODO: Fix single dining philosopher
static bool	philo_take_forks(t_philo *philo)
{
	if (table_running())
	{
		sem_wait(g_table.fork_count);
		if (table_running())
		{
			table_log(philo, "has taken a fork");
			sem_wait(g_table.fork_count);
			if (table_running())
			{
				table_log(philo, "has taken a fork");
				return (true);
			}
			sem_post(g_table.fork_count);
		}
		sem_post(g_table.fork_count);
	}
	return (false);
}

static void	philo_drop_forks(void)
{
	sem_post(g_table.fork_count);
	sem_post(g_table.fork_count);
}

bool	philo_eat(t_philo *philo)
{
	bool	running;

	if ((running = philo_take_forks(philo)))
	{
		sem_wait(philo->lock);
		philo->time_die = time_millis() + g_table.time_to_die;
		sem_post(philo->lock);
		// TODO: May need to check running state again...
		running = philo_sleep(philo, g_table.time_to_eat, "is eating");
		philo_drop_forks();
		if (running && g_table.appetite
		&& philo->times_ate < g_table.appetite
		&& ++philo->times_ate == g_table.appetite)
		{
			sem_wait(g_table.lock_run);
			if (++g_table.satisfied == g_table.seats)
			{
				g_table.running = false;
				running = false;
			}
			sem_post(g_table.lock_run);
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
		sem_wait(philo->lock);
		time_die = philo->time_die;
		sem_post(philo->lock);
		now = table_log(philo, message);
		if (now + duration > time_die)
			duration = time_die - now;
		usleep(duration * 1000);
	}
	return (running);
}

bool	philo_think(t_philo *philo)
{
	bool	running;

	if ((running = table_running()))
		table_log(philo, "is thinking");
	return (running);
}
