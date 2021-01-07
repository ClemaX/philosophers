#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>

# include <utils.h>
# include <table.h>

typedef struct	s_philo
{
	pthread_mutex_t	lock;
	t_uint			index;
	t_uint			forks[2];
	pthread_t		tid;
	pthread_t		tid_observer;
	t_time			time_die;
	t_uint			times_ate;
}				t_philo;

/*
**				philo_actions.c
*/
bool			philo_eat(t_philo *philo);
bool			philo_sleep(t_philo *philo, t_time duration,
	const char *message);
bool			philo_think(t_philo *philo);

bool			philo_set(t_philo *philo, t_uint index);
void			*philo_thread(void *data);

#endif
