#include <table.h>

int				main(int ac, char **av)
{
	t_table	table;
	t_philo	*philos;

	if (!table_set(&table, &philos, ac, av))
		return (1);
	if (table_start(&table, philos))
		table_join(&table, philos);
	table_clear(&table, &philos);
	return (0);
}
