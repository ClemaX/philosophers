#include <observer.h>

/*
**	Observe a dining philosopher.
**
**	data: A pointer to the philosopher to be observed.
**
**	0. Initialize the philosopher's death time.
**	1. Wait until the philosopher should die.
**	2. Check if the table stopped running or the philosopher died.
*/

void	*observer_thread(void *data)
{
	t_philo	*const	philo = data;
	bool			alive;
	bool			running;
	bool			satisfied;

	alive = true;
	running = true;
	satisfied = false;
	while (running && !satisfied && alive)
	{
		usleep(philo->table->time_to_die * 1000);
		sem_wait(philo->lock);
		if (!(satisfied = philo->times_ate == philo->table->appetite)
		&& !(alive = (clock_millis() < philo->time_die)))
		{
			sem_wait(philo->table->lock_run);
			if (philo->table->running)
			{
				philo_log(philo, "died");
				philo->table->running = false;
			}
			running = false;
			sem_post(philo->table->lock_run);
		}
		sem_post(philo->lock);
	}
	// TODO: Remove this debug log
	if (satisfied)
		philo_log(philo, "is satisfied");
	pthread_join(philo->tid, NULL);
	return (NULL);
}
