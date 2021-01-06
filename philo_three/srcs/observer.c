#include <philo.h>

static bool	observe_death(t_philo *philo)
{
	t_time	time_die;
	bool	running;

	sem_wait(philo->lock);
	time_die = philo->time_die;
	sem_post(philo->lock);
	sleep_until(time_die);
	sem_wait(philo->lock);
	if (!(running = time_die != philo->time_die))
	{
		sem_wait(g_table.lock_run);
		table_log(philo, "died");
		table_stop();
	}
	sem_post(philo->lock);
	return (running);
}

void		*observer_thread(void *data)
{
	t_philo *const	philo = data;

	while (observe_death(philo))
		;
	return (data);
}

bool		observer_start(t_philo *philo)
{
	const int	err = pthread_create(&philo->tid_observer, NULL,
		&observer_thread, philo);

	if (!err)
		return (true);
	table_perror("pthread_create", err);
	return (false);
}
