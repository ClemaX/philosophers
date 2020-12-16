#ifndef TABLE_H
# define TABLE_H

# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>

# include <philo.h>

typedef struct	s_table
{
	t_philo 	*philos;
	
	uint64_t	seats;
	uint64_t	appetite;
	uint64_t	time_start;
	uint64_t	time_die;
	uint64_t	time_eat;
	uint64_t	time_sleep;
}				t_table;

bool			table_set(t_table *table, int ac, char **av);
void			table_start(t_table *table);
int				table_log(t_table *table, const char *message);

#endif