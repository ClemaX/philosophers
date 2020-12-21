#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include <utils.h>
# include <table.h>

typedef struct s_table t_table;

typedef struct	s_philo
{
	pthread_mutex_t	lock;
	t_table			*table;
	uint64_t		index;
	pthread_t		tid;
	t_time			time_die;
	uint64_t		times_ate;
}				t_philo;

bool	philo_log(t_philo *philo, const char *message);
void	*philo_thread(void *data);

#endif
