#include <philo.h>

bool	philo_set(t_philo *philo, t_uint index)
{
	int	err;

	if (!(err = pthread_mutex_init(&philo->lock_state, NULL)))
	{
		ft_strcpy(philo->log_buffer, MSG_TEMPLATE);
		philo->forks[0] = &g_table.forks[index];
		philo->forks[1] = &g_table.forks[(index + 1) % (g_table.seats)];
		philo->index = index;
		// TODO: philo->times_ate = 0;
		philo->time_starve = 0;
		philo->state = TAKING_FORK;
		philo->state_changed = false;

		//dprintf(2, "philo %llu using forks %llu and %llu!\n", index + 1, philo->forks[0], philo->forks[1]);
	}
	return (!err);
}

static void	philo_set_state(t_philo *philo, t_philo_state state)
{
	pthread_mutex_lock(&philo->lock_state);
	philo->state = state;
	philo->state_changed = true;
	pthread_mutex_unlock(&philo->lock_state);
}

static void	philo_eat(t_philo *philo)
{
	const t_time	now = time_millis();

	pthread_mutex_lock(&philo->lock_state);
	philo->time_starve = now + g_table.time_to_starve;
	philo->state = EATING;
	philo->state_changed = true;
	pthread_mutex_unlock(&philo->lock_state);
	// EAT
	usleep(g_table.time_to_eat * 1000);
}

static void	philo_sleep(t_philo *philo)
{
	const t_time	time_wake = time_millis() + g_table.time_to_sleep;

	pthread_mutex_lock(&philo->lock_state);
	philo->state = SLEEPING;
	philo->state_changed = true;
	pthread_mutex_unlock(&philo->lock_state);
	sleep_until(time_wake);
}

void	*philo_thread(void *data)
{
	t_philo *const	philo = data;

	while (table_running())
	{
		// TAKE FORKS
		pthread_mutex_lock(philo->forks[0]);
		philo_set_state(philo, TAKING_FORK);
		pthread_mutex_lock(philo->forks[1]);
		// EAT
		philo_eat(philo);
		pthread_mutex_unlock(philo->forks[1]);
		pthread_mutex_unlock(philo->forks[0]);
		// SLEEP
		philo_sleep(philo);
		// THINK
		philo_set_state(philo, THINKING);
	}
	return (data);
}
