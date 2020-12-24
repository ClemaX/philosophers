#include <philo.h>

void		philo_log(t_philo *philo, const char *message)
{
	const t_time	now = clock_millis() - philo->table->time_start;

	sem_wait(philo->table->lock_write);
	putui(STDOUT_FILENO, now);
	write(STDOUT_FILENO, " ", 1);
	putui(STDOUT_FILENO, philo->index + 1);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
	sem_post(philo->table->lock_write);
}

void		*philo_thread(void *data)
{
	t_philo *const	philo = data;

	// FIXME: Lock is null!
	sem_wait(philo->lock);
	philo->times_ate = 0;
	philo->time_die = clock_millis() + philo->table->time_to_die;
	sem_post(philo->lock);
	while (philo_eat(philo) && philo_sleep(philo) && philo_think(philo))
		;
	return (NULL);
}

const char	*philo_sem_name(uint64_t index)
{
	static char			name[NAME_MAX - 3] = SEM_PHILO_PREFIX;
	char *const			suffix = name + sizeof(SEM_PHILO_PREFIX) - 1;
	unsigned char		id_len;
	const char *const	id = uitoa(index, &id_len);

	if (sizeof(SEM_PHILO_PREFIX) + id_len > sizeof(name))
		return (NULL);
	suffix[id_len] = '\0';
	while (id_len--)
		suffix[id_len] = id[id_len];
	return (name);
}

bool		philo_new(t_philo *philo, t_table *table, uint64_t index)
{
	const char *const	sem_name = philo_sem_name(index);

	philo->index = index;
	if ((philo->lock = sem_open(sem_name, SEM_OFLAGS, SEM_MODE, 1)) != SEM_FAILED)
	{
		philo->table = table;
		return (true);
	}
	perror(sem_name);
	return (false);
}

void		philo_del(t_philo *philo)
{
	sem_close(philo->lock);
	sem_unlink(philo_sem_name(philo->index));
}
