#include <observer.h>

void	*observer_thread(void *data)
{
	t_philo *const	philo = data;
	t_time			time_die;
	bool			running;

	running = true;
	while (running)
	{
		sem_wait(philo->lock);
		time_die = philo->time_die;
		sem_post(philo->lock);
		sleep_until(time_die);
		sem_wait(philo->lock);
		if (time_die == philo->time_die)
		{
			sem_wait(g_table.lock_run);
			if (g_table.running)
			{
				table_log(philo, "died");
				g_table.running = false;
			}
			sem_post(g_table.lock_run);
			running = false;
		}
		sem_post(philo->lock);
	}
	pthread_join(philo->tid, &data);
	return (data);
}
