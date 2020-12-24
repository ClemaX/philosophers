#include <table.h>

void		show_usage(const char *name)
{
	write(2, MSG_EUSAGE_PREFIX, sizeof(MSG_EUSAGE_PREFIX) - 1);
	write(2, name, ft_strlen(name));
	write(2, MSG_EUSAGE_SUFFIX, sizeof(MSG_EUSAGE_SUFFIX) - 1);
}

bool		table_running(void)
{
	bool	running;

	pthread_mutex_lock(&g_table.lock_run);
	running = g_table.running;
	pthread_mutex_unlock(&g_table.lock_run);
	return (running);
}

bool		table_new(t_philo **philos, int ac, char **av)
{
	g_table.appetite = 0;
	if ((ac == 5 || (ac == 6 && (g_table.appetite = atoui(av[5]))))
	&& (g_table.seats = atoui(av[1]))
	&& (g_table.time_to_die = atoui(av[2]))
	&& (g_table.time_to_eat = atoui(av[3]))
	&& (g_table.time_to_sleep = atoui(av[4])))
	{
		if ((*philos = malloc(sizeof(**philos) * g_table.seats)))
		{
			if ((g_table.forks = malloc(sizeof(*g_table.forks) * g_table.seats)))
				return (true);
			free(*philos);
		}
		write(STDERR_FILENO, MSG_EALLOC, sizeof(MSG_EALLOC) - 1);
	}
	else
		show_usage(av[0]);
	return (false);
}

void		table_del(t_philo **philos)
{
	uint64_t	i;

	i = 0;
	while (i < g_table.seats)
	{
		pthread_mutex_destroy(&g_table.forks[i]);
		pthread_mutex_destroy(&(*philos)[i++].lock);
	}
	pthread_mutex_destroy(&g_table.lock_run);
	pthread_mutex_destroy(&g_table.lock_write);
	free(g_table.forks);
	free(*philos);
	g_table.forks = NULL;
	*philos = NULL;
}

// TODO: Handle only memory allocation errors
static bool	table_set(t_philo *philos)
{
	uint64_t	i;
	int			err;

	if (!(err = pthread_mutex_init(&g_table.lock_write, NULL))
	&& !(err = pthread_mutex_init(&g_table.lock_run, NULL)))
	{
		i = 0;
		while (i < g_table.seats
		&& !(err = pthread_mutex_init(&g_table.forks[i], NULL)))
		{
			philos[i].index = i;
			i++;
		}
		if (!err)
			return (true);
	}
	return (false);
}

bool		table_start(t_philo *philos)
{
	uint64_t			i;
	int					err;

	if (!table_set(philos))
		return (false);
	i = 0;
	err = 0;
	g_table.running = true;
	g_table.time_start = clock_millis();
	while (i < g_table.seats
	&& !(err = pthread_create(&philos[i].tid, NULL, &philo_thread, &philos[i]))
	&& !(err = pthread_create(&philos[i].tid_observer, NULL, &observer_thread, &philos[i])))
		i++;
	return (!err);
}

bool		table_join(t_philo *philos)
{
	uint64_t	i;
	int			err;

	i = 0;
	err = 0;
	while (i < g_table.seats
	&& !(err = pthread_join(philos[i].tid_observer, NULL)))
		i++;
	return (!err);
}
