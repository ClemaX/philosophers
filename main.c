#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct	s_philos
{
	
}				t_philos;

typedef struct	s_table
{
	t_philos	*philos;
	uint64_t	seats;
	uint64_t	appetite;
	uint64_t	time_start;
	uint64_t	time_die;
	uint64_t	time_eat;
	uint64_t	time_sleep;
}				t_table;

static uint64_t	clock_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (uint64_t)1000 + now.tv_usec / 1000);
}

static uint64_t	atoui(const char *str)
{
	uint64_t	i;

	i = 0;
	while (*str >= '0' && *str <= '9' && i < INT64_MAX)
		i = i * 10 + *str++ - '0';
	return ((i >= INT64_MAX) ? 0 : i);
}

void			show_usage(const char *name)
{
	dprintf(2, "Usage:	%s seats time_die time_eat time_sleep [appetite]\n", name);
}

bool			table_set(t_table *table, int ac, char **av)
{
	if (ac == 6)
		table->appetite = atoui(av[5]);
	else if (ac == 5)
		table->appetite = 1;
	else
	{
		show_usage(av[0]);
		return (false);
	}
	if (table->appetite && (table->seats = atoui(av[1]))
	&& (table->time_die = atoui(av[2]))
	&& (table->time_eat = atoui(av[3]))
	&& (table->time_sleep = atoui(av[4])))
	{
		if (!(table->philos = malloc(sizeof(*table->philos) * table->seats)))
		{
			perror(av[0]);
			return (false);
		}
		// TODO: Initialize philos
		return (true);
	}
	show_usage(av[0]);
	return (false);
}

void			table_start(t_table *table)
{
	table->time_start = clock_millis();
}

int				main(int ac, char **av)
{
	t_table	table;

	if (!table_set(&table, ac, av))
		return (1);
	table_start(&table);
	dprintf(STDERR_FILENO, "%lu %s\n", clock_millis() - table.time_start, "Start");
	return (0);
}
