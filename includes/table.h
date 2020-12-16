#ifndef TABLE_H
# define TABLE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <inttypes.h>
# include <string.h>

# include <utils.h>
# include <philo.h>

typedef struct	s_philo	t_philo;

typedef struct	s_table
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	uint64_t		seats;
	uint64_t		appetite;
	t_time			time_start;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
}				t_table;

bool			table_set(t_table *table, t_philo **philos, int ac, char **av);
bool			table_start(t_table *table, t_philo *philos);
int				table_log(t_table *table, const char *message);
void			table_clear(t_table *table, t_philo **philos);
bool			table_join(t_table *table, t_philo *philos);

#endif
