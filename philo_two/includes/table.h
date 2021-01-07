#ifndef TABLE_H
# define TABLE_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

# include <sys/stat.h>
# include <limits.h>

# include <fcntl.h>
# include <semaphore.h>

# include <utils.h>
# include <philo.h>
# include <observer.h>

# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

# ifndef FW_TIMESTAMP
#  define FW_TIMESTAMP 8
# endif

# define MSG_EUSAGE_PREFIX	"Usage: "
# define MSG_EUSAGE_ARGS	" seats time_to_die time_to_eat time_to_sleep"
# define MSG_EUSAGE_OPTS	" [appetite]"
# define MSG_EUSAGE_SUFFIX	"\n"

# define SEM_OFLAGS			O_CREAT
# define SEM_MODE			0000700

# define SEM_FORK_COUNT		"/table_forks"
# define SEM_WRITE			"/table_write"
# define SEM_RUN			"/table_run"

typedef struct s_philo	t_philo;

typedef struct			s_table
{
	bool			running;
	sem_t			*fork_count;
	sem_t			*lock_write;
	sem_t			*lock_run;
	t_uint			seats;
	t_uint			appetite;
	t_uint			satisfied;
	t_time			time_start;
	t_time			time_to_die;
	t_time			time_to_eat;
	t_time			time_to_sleep;
	unsigned char	fw_index;
}						t_table;

extern t_table			g_table;

void					table_show_usage(const char *name);
void					table_perror(const char *label, int err);
t_time					table_log(t_philo *philo, const char *message);

bool					table_running(void);

bool					table_new(t_philo **philos, int ac, const char **av);
void					table_del(t_philo **philos);

bool					table_start(t_philo *philos);
bool					table_join(t_philo *philos);

#endif
