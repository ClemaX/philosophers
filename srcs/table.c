#include <table.h>

// TODO: Replace printfs with writes

void			show_usage(const char *name)
{
	dprintf(2, "Usage:	%s seats time_to_die time_to_eat time_to_sleep [appetite]\n",
		name);
}

bool			table_running(t_table *table)
{
	bool	running;

	pthread_mutex_lock(&table->lock_run);
	running = table->running;
	pthread_mutex_unlock(&table->lock_run);
	return (running);
}

bool			table_new(t_table *table, int ac, char **av)
{
	table->appetite = 0;
	if ((ac == 5 || (ac == 6 && (table->appetite = atoui(av[5]))))
	&& (table->seats = atoui(av[1]))
	&& (table->time_to_die = atoui(av[2]))
	&& (table->time_to_eat = atoui(av[3]))
	&& (table->time_to_sleep = atoui(av[4])))
	{
		if ((table->philos = malloc(sizeof(*table->philos) * table->seats)))
		{
			if ((table->observers = malloc(sizeof(*table->observers) * table->seats)))
			{
				if ((table->forks = malloc(sizeof(*table->forks) * table->seats)))
					return (true);
				free(table->observers);
			}
			free(table->philos);
		}
		perror("malloc");
	}
	else
		show_usage(av[0]);
	return (false);
}

void			table_clear(t_table *table)
{
	uint64_t	i;

	i = 0;
	while (i < table->seats)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i++].lock);
	}
	pthread_mutex_destroy(&table->lock_run);
	pthread_mutex_destroy(&table->lock_write);
	free(table->forks);
	free(table->philos);
	free(table->observers);
	table->forks = NULL;
	table->philos = NULL;
	table->observers = NULL;
}

// TODO: Handle only memory allocation errors
bool			table_set(t_table *table)
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
			table->philos[i].table = table;
			table->philos[i].index = i;
			i++;
		}
		if (!err)
			return (true);
	}
	dprintf(STDERR_FILENO, "pthread_mutex_init: (%d)%s\n", err, strerror(err));
	return (false);
}

bool			table_start(t_table *table)
{
	uint64_t			i;
	int					err;

	table_set(table);
	i = 0;
	err = 0;
	table->running = true;
	table->time_start = clock_millis();
	while (i < table->seats
	&& !(err = pthread_create(&table->philos[i].tid, NULL, &philo_thread, &table->philos[i]))
	&& !(err = pthread_create(&table->observers[i], NULL, &observer_thread, &table->philos[i])))
		i++;
	if (err)
	{
		dprintf(STDERR_FILENO, "pthread_create: (%d)%s\n", err, strerror(err));
		return (false);
	}
	return (true);
}

bool			table_join(t_table *table)
{
	uint64_t	i;
	int			err;

	i = 0;
	err = 0;
	while (i < table->seats
	&& !(err = pthread_join(table->observers[i], NULL)))
		i++;
	if (err)
	{
		dprintf(STDERR_FILENO, "pthread_join: (%d)%s\n", err, strerror(err));
		return (false);
	}
	return (true);
}
