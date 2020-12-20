#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include <utils.h>
# include <table.h>

typedef struct s_table t_table;

typedef struct	s_philo
{
	t_table		*table;
	size_t		index;
	pthread_t	tid;
	t_time		time_die;
}				t_philo;

void	*philo_thread(void *data);

#endif
