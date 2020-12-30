#include <table.h>

bool			table_new(t_philo **philos, int ac, const char **av)
{
	g_table.appetite = 0;
	if ((ac == 5 || (ac == 6 && (g_table.appetite = atoui(av[5]))))
	&& (g_table.seats = atoui(av[1]))
	&& (g_table.time_to_die = atoui(av[2]))
	&& (g_table.time_to_eat = atoui(av[3]))
	&& (g_table.time_to_sleep = atoui(av[4])))
	{
		if ((*philos = malloc(sizeof(**philos) * g_table.seats)))
			return (true);
		table_perror("table: malloc", errno);
	}
	else
		table_show_usage(av[0]);
	return (false);
}

void			table_del(t_philo **philos)
{
	t_uint	i;

	i = 0;
	while (i < g_table.seats)
		philo_del(&(*philos)[i++]);
	sem_close(g_table.fork_count);
	sem_close(g_table.lock_run);
	sem_close(g_table.lock_write);
	sem_unlink(SEM_FORK_COUNT);
	sem_unlink(SEM_RUN);
	sem_unlink(SEM_WRITE);
	free(*philos);
	*philos = NULL;
	g_table.fork_count = SEM_FAILED;
	g_table.lock_run = SEM_FAILED;
	g_table.lock_write = SEM_FAILED;
}

static bool		table_set(t_philo *philos)
{
	t_uint	i;

	i = 0;
	if ((g_table.fork_count = sem_open(SEM_FORK_COUNT, SEM_OFLAGS, SEM_MODE,
		g_table.seats)) != SEM_FAILED)
	{
		if ((g_table.lock_write = sem_open(SEM_WRITE, SEM_OFLAGS, SEM_MODE, 1))
			!= SEM_FAILED)
		{
			if ((g_table.lock_run = sem_open(SEM_RUN, SEM_OFLAGS, SEM_MODE, 1))
				!= SEM_FAILED)
			{
				while (i < g_table.seats && philo_set(&philos[i], i))
					i++;
				if (i == g_table.seats)
					return (true);
			}
		}
	}
	g_table.seats = i;
	table_perror("table: semaphores", errno);
	return (false);
}

bool			table_start(t_philo *philos)
{
	t_uint	i;
	int		err;

	if (!table_set(philos))
		return (false);
	i = 0;
	err = 0;
	g_table.running = true;
	g_table.time_start = time_millis();
	while (!err && i < g_table.seats)
	{
		philos[i].time_die = g_table.time_start + g_table.time_to_die;
		if (!(err = pthread_create(&philos[i].tid, NULL,
			&philo_thread, &philos[i])))
			err = pthread_create(&philos[i].tid_observer, NULL,
				&observer_thread, &philos[i]);
		i++;
	}
	if (i != g_table.seats)
		table_perror("table: pthread_create", err);
	return (!err);
}

bool			table_join(t_philo *philos)
{
	t_uint	i;
	int			err;

	i = 0;
	err = 0;
	while (i < g_table.seats
	&& !(err = pthread_join(philos[i].tid_observer, NULL)))
		i++;
	if (i != g_table.seats)
		table_perror("pthread_join", err);
	return (!err);
}
