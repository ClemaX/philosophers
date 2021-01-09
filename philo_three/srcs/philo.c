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

bool				philo_start(t_philo *philo)
{
	if ((philo->pid = fork()) == 0)
	{
		philo->time_die = g_table.time_start + g_table.time_to_die;
		if (observer_start(philo))
		{
			philo_thread(philo);
			exit(0);
		}
		exit(1);
	}
	else if (philo->pid == -1)
	{
		table_perror("fork", errno);
		return (false);
	}
	return (true);
}

void				*philo_thread(void *data)
{
	t_philo *const	philo = data;
	int				err;

	while (philo_eat(philo)
	&& philo_sleep(philo, g_table.time_to_sleep, "is sleeping")
	&& philo_think(philo))
		;
	if ((err = pthread_join(philo->tid_observer, NULL)))
		table_perror("pthread_join", err);
	return (NULL);
}
