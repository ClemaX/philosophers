#include <table.h>

// TODO: Replace printfs with writes

void			show_usage(const char *name)
{
	dprintf(2, "Usage:	%s seats time_to_die time_to_eat time_to_sleep [appetite]\n",
		name);
}

bool			table_new(t_table *table, t_philo **philos, int ac, char **av)
{
	table->appetite = 0;
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
	uint64_t	i;

	i = 0;
	while (i < table->seats)
		pthread_mutex_destroy(&table->forks[i++]);
	pthread_mutex_destroy(&table->lock_run);
	pthread_mutex_destroy(&table->lock_write);
	free(table->forks);
	free(*philos);
	table->forks = NULL;
	*philos = NULL;
}

bool			table_set(t_table *table, t_philo *philos)
{
	uint64_t	i;
	int			err;

	if (!(err = pthread_mutex_init(&table->lock_write, NULL))
	&& !(err = pthread_mutex_init(&table->lock_run, NULL)))
	{
		i = 0;
		while (i < table->seats
		&& !(err = pthread_mutex_init(&table->forks[i], NULL)))
		{
			philos[i].table = table;
			philos[i].index = i;
			i++;
		}
		if (!err)
			return (true);
	}
	dprintf(STDERR_FILENO, "pthread_mutex_init: (%d)%s\n", err, strerror(err));
	return (false);
}

bool			table_start(t_table *table, t_philo *philos)
{
	uint64_t			i;
	int					err;

	table_set(table, philos);
	i = 0;
	err = 0;
	table->running = true;
	table->time_start = clock_millis();
	while (i < table->seats
	&& !(err = pthread_create(&philos[i].tid, NULL, &philo_thread, &philos[i])))
		i++;
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
