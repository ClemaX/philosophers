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
	sem_close(g_table.count_forks);
	sem_close(g_table.lock_run);
	sem_close(g_table.lock_write);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_RUN);
	sem_unlink(SEM_WRITE);
	free(*philos);
	*philos = NULL;
	g_table.count_forks = SEM_FAILED;
	g_table.lock_run = SEM_FAILED;
	g_table.lock_write = SEM_FAILED;
}

bool			table_set(t_philo *philos)
{
	t_uint	i;

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_SATISFIED);
	sem_unlink(SEM_WRITE);
	sem_unlink(SEM_RUN);
	i = 0;
	if ((g_table.count_forks = sem_open(SEM_FORKS, SEM_OFLAGS, SEM_MODE,
		g_table.seats)) != SEM_FAILED
	&& (g_table.count_satisified = sem_open(SEM_SATISFIED, SEM_OFLAGS, SEM_MODE,
		0)) != SEM_FAILED
	&& (g_table.lock_write = sem_open(SEM_WRITE, SEM_OFLAGS, SEM_MODE, 1))
		!= SEM_FAILED
	&& (g_table.lock_run = sem_open(SEM_RUN, SEM_OFLAGS, SEM_MODE, 1))
		!= SEM_FAILED)
	{
		while (i < g_table.seats && philo_set(&philos[i], i))
			i++;
		if (i == g_table.seats)
			return (true);
	}
	g_table.seats = i;
	table_perror("table: sem_open", errno);
	return (false);
}

bool			table_join(t_philo *philos)
{
	t_uint	i;
	int		status;

	i = 0;
	while (i < g_table.seats && sem_wait(g_table.count_satisified) != -1)
		i++;
	if (i != g_table.seats)
		table_perror("table: sem_wait", errno);
	i = 0;
	while (i < g_table.seats && kill(philos[i].pid, SIGTERM) != -1)
		i++;
	sem_post(g_table.lock_run);
	if (i != g_table.seats)
	{
		table_perror("table: kill", errno);
		return (false);
	}
	i = 0;
	while (i < g_table.seats && waitpid(philos[i].pid, &status, 0))
	{
		philos[i].pid = 0;
		i++;
	}
	return (true);
}
