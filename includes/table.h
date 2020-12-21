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
# include <observer.h>

# define	MSG_EUSAGE_PREFIX	"Usage: "
# define	MSG_EUSAGE_SUFFIX	" seats time_to_die time_to_eat time_to_sleep [appetite]\n"

# define	MSG_EALLOC	"Allocation error!\n"

typedef struct s_philo	t_philo;

typedef struct			s_table
{
	t_philo			*philos;
	pthread_t		*observers;
	// TODO: Substruct table_shared
	// TODO: Otherwise consider adding observer tid to philo struct
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock_write;
	pthread_mutex_t	lock_run;
	bool			running;
	uint64_t		seats;
	uint64_t		appetite;
	t_time			time_start;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
}						t_table;

// TODO: Remove unused function definitions
bool					table_running(t_table *table);

bool					table_new(t_table *table, int ac, char **av);
bool					table_set(t_table *table);
bool					table_start(t_table *table);
void					table_clear(t_table *table);
bool					table_join(t_table *table);

#endif
