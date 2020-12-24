#include <table.h>

int				main(int ac, char **av)
{
	t_table	table;

	if (!table_new(&table, ac, av))
		return (1);
	if (table_start(&table))
		table_join(&table);
	table_clear(&table);
	return (0);
}
