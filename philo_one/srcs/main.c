#include <table.h>

t_table	g_table = (t_table){ 0 };

int				main(int ac, char **av)
{
	t_philo	*philos;

	if (!table_new(&philos, ac, av))
		return (1);
	if (table_start(philos))
		table_join(philos);
	table_del(&philos);
	return (0);
}
