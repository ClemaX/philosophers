#include <table.h>

int				main(int ac, char **av)
{
	t_table	table;

	if (!table_set(&table, ac, av))
		return (1);
	table_start(&table);
	table_log(&table, "Start");
	return (0);
}
