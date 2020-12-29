#include <observer.h>

void	*observer_thread(void *data)
{
	t_philo *const	philo = data;
	t_time			time_die;
	bool			running;

	running = true;
	while (running)
	{
		pthread_mutex_lock(&philo->lock);
		time_die = philo->time_die;
		pthread_mutex_unlock(&philo->lock);
		sleep_until(time_die);
		pthread_mutex_lock(&philo->lock);
		if (time_die == philo->time_die)
		{
			pthread_mutex_lock(&g_table.lock_run);
			if (g_table.running)
			{
				table_log(philo, "died");
				g_table.running = false;
			}
			pthread_mutex_unlock(&g_table.lock_run);
			running = false;
		}
		pthread_mutex_unlock(&philo->lock);
	}
	pthread_join(philo->tid, &data);
	return (data);
}
