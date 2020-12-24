#include <table.h>

void			show_usage(const char *name)
{
	write(STDERR_FILENO, MSG_EUSAGE_PREFIX, sizeof(MSG_EUSAGE_PREFIX) - 1);
	write(STDERR_FILENO, name, ft_strlen(name));
	write(STDERR_FILENO, MSG_EUSAGE_SUFFIX, sizeof(MSG_EUSAGE_SUFFIX) - 1);
}

bool			table_running(t_table *table)
{
	bool	running;

	sem_wait(table->lock_run);
	running = table->running;
	sem_post(table->lock_run);
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
				return (true);
			free(table->philos);
		}
		write(STDERR_FILENO, MSG_EALLOC, sizeof(MSG_EALLOC) - 1);
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
		philo_del(&table->philos[i++]);
	sem_close(table->lock_fork_count);
	sem_close(table->lock_run);
	sem_close(table->lock_write);
	sem_unlink(SEM_FORK_COUNT);
	sem_unlink(SEM_RUN);
	sem_unlink(SEM_WRITE);
	free(table->philos);
	free(table->observers);
	table->philos = NULL;
	table->observers = NULL;
	table->lock_fork_count = SEM_FAILED;
	table->lock_run = SEM_FAILED;
	table->lock_write = SEM_FAILED;
}

bool			table_set(t_table *table)
{
	uint64_t	i;

	i = 0;
	if ((table->lock_fork_count = sem_open(SEM_FORK_COUNT, SEM_OFLAGS, SEM_MODE, 1)) != SEM_FAILED)
	{
		if ((table->lock_write = sem_open(SEM_WRITE, SEM_OFLAGS, SEM_MODE, 1)) != SEM_FAILED)
		{
			if ((table->lock_run = sem_open(SEM_RUN, SEM_OFLAGS, SEM_MODE, 1)) != SEM_FAILED)
			{
				while (i < table->seats && philo_new(&table->philos[i], table, i))
					i++;
				if (i == table->seats)
					return (true);
			}
			sem_close(table->lock_write);
			sem_unlink(SEM_WRITE);
		}
		sem_close(table->lock_fork_count);
		sem_unlink(SEM_FORK_COUNT);
	}
	table->seats = i;
	perror("table: semaphores");
	return (false);
}

bool			table_start(t_table *table)
{
	uint64_t			i;
	int					err;

	if (!table_set(table))
		return (false);
	i = 0;
	err = 0;
	table->running = true;
	table->time_start = clock_millis();
	while (i < table->seats
	&& !(err = pthread_create(&table->philos[i].tid, NULL, &philo_thread, &table->philos[i]))
	&& !(err = pthread_create(&table->observers[i], NULL, &observer_thread, &table->philos[i])))
		i++;
	return (!err);
}

bool			table_join(t_table *table)
{
	uint64_t	i;
	int			err;

	i = 0;
	err = 0;
	while (i < table->seats && !(err = pthread_join(table->observers[i], NULL)))
		i++;
	return (!err);
}
