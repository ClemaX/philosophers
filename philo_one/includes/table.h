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

extern t_table			g_table;

// TODO: Remove unused function definitions
bool					table_running(void);

bool					table_new(t_philo **philos, int ac, char **av);
void					table_del(t_philo **philos);

bool					table_start(t_philo *philos);
bool					table_join(t_philo *philos);

#endif
