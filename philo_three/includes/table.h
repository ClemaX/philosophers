#ifndef TABLE_H
# define TABLE_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <signal.h>

# include <sys/stat.h>
# include <limits.h>

# include <fcntl.h>
# include <semaphore.h>

# include <utils.h>
# include <philo.h>

# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

# ifndef FW_TIMESTAMP
#  define FW_TIMESTAMP 8
# endif

# define MSG_EUSAGE_PREFIX	"Usage: "
# define MSG_EUSAGE_ARGS	" seats time_to_die time_to_eat time_to_sleep"
# define MSG_EUSAGE_OPTS	" [appetite]"
# define MSG_EUSAGE_SUFFIX	"\nThe number of seats must be greater than 1.\n"

# define SEM_OFLAGS			O_CREAT
# define SEM_MODE			0000700

# define SEM_FORKS			"/table_forks"
# define SEM_SATISFIED		"/table_satisfied"
# define SEM_WRITE			"/table_write"
# define SEM_RUN			"/table_run"

typedef struct s_philo	t_philo;

typedef struct			s_table
{
	sem_t			*lock_write;
	sem_t			*lock_run;
	sem_t			*count_forks;
	sem_t			*count_satisified;
	t_uint			seats;
	t_uint			appetite;
	t_time			time_start;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
	unsigned char	fw_index;
}						t_table;

extern t_table			g_table;

/*
**						table_utils.c
*/
void					table_show_usage(const char *name);
void					table_perror(const char *label, int err);
t_time					table_log(t_philo *philo, const char *message);

/*
**						table.c
*/
bool					table_set(t_philo *philos);
bool					table_new(t_philo **philos, int ac, const char **av);
void					table_del(t_philo **philos);
bool					table_join(t_philo *philos);

/*
**						table_actions.c
*/
bool					table_start(t_philo *philos);
void					table_stop(void);
bool					table_take_fork(t_philo *philo);
void					table_drop_fork(void);
bool					table_running(void);

#endif
