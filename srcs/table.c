#include <table.h>

void			show_usage(const char *name)
{
	dprintf(2, "Usage:	%s seats time_die time_eat time_sleep [appetite]\n",
		name);
}

bool			table_set(t_table *table, t_philo **philos, int ac, char **av)
{
	table->appetite = 1;
	if ((ac == 5 || (ac == 6 && (table->appetite = atoui(av[5]))))
	&& (table->seats = atoui(av[1]))
	&& (table->time_to_die = atoui(av[2]))
	&& (table->time_to_eat = atoui(av[3]))
	&& (table->time_to_sleep = atoui(av[4])))
	{
		if ((*philos = malloc(sizeof(**philos) * table->seats)))
		{
			if ((table->forks = malloc(sizeof(*table->forks) * table->seats)))
				return (true);
			free(*philos);
		}
		perror("malloc");
	}
	else
		show_usage(av[0]);
	return (false);
}

void			table_clear(t_table *table, t_philo **philos)
{
	free(table->forks);
	free(*philos);
	table->forks = NULL;
	*philos = NULL;
}

bool			table_start(t_table *table, t_philo *philos)
{
	uint64_t			i;
	pthread_attr_t		thread_attr;
	//pthread_mutexattr_t	mutex_attr;
	int					err;

	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&table->write_lock, NULL);
	i = 0;
	err = 0;
	table->time_start = clock_millis();
	while (!err && i < table->seats)
	{
		philos[i].index = i;
		philos[i].table = table;
		err = pthread_create(&philos[i].tid, &thread_attr, &philo_thread, &philos[i]);
		i++;
	}
	if (err)
	{
		dprintf(STDERR_FILENO, "pthread_create: (%d)%s\n", err, strerror(err));
		// TODO: Investigate pthread cancelation and cleanup
		// TODO: Join philos before freeing data
		return (false);
	}
	return (true);
}

bool			table_join(t_table *table, t_philo *philos)
{
	uint64_t	i;
	int			err;

	i = 0;
	err = 0;
	while (i < table->seats
	&& !(err = pthread_join(philos[i].tid, NULL)))
		i++;
	if (err)
	{
		dprintf(STDERR_FILENO, "pthread_join: (%d)%s\n", err, strerror(err));
		return (false);
	}
	return (true);
}
