#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include <table.h>
# include <utils.h>

typedef struct	s_philo
{
	pthread_mutex_t	lock;
	uint64_t		index;
	pthread_t		tid;
	pthread_t		tid_observer;
	t_time			time_die;
	uint64_t		times_ate;
}				t_philo;

bool	philo_log(t_philo *philo, const char *message);
void	*philo_thread(void *data);

#endif
