#include <philosophers.h>

int	main(int ac, const char **av)
{
	t_philo	*philos;
	bool	success;

	if (!table_new(&philos, ac, av))
		return (1);
	success = false;
	if (table_start(philos))
		success = table_join(philos);
	table_del(&philos);
	return (success ? 0 : -1);
}
