#include <philo.h>

static const char	*philo_sem_name(t_uint index)
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

bool				philo_set(t_philo *philo, t_uint index)
{
	const char *const	sem_name = philo_sem_name(index);

	sem_unlink(sem_name);
	if ((philo->lock = sem_open(sem_name, SEM_OFLAGS, SEM_MODE, 1))
		!= SEM_FAILED)
	{
		philo->index = index;
		philo->times_ate = 0;
		philo->time_die = 0;
		return (true);
	}
	table_perror(sem_name, errno);
	return (false);
}

void				philo_del(t_philo *philo)
{
	sem_close(philo->lock);
	sem_unlink(philo_sem_name(philo->index));
}

void				*philo_thread(void *data)
{
	t_philo *const	philo = data;

	while (philo_eat(philo)
	&& philo_sleep(philo, g_table.time_to_sleep, "is sleeping")
	&& philo_think(philo))
		;
	return (NULL);
}
