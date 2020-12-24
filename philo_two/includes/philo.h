#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include <utils.h>
# include <table.h>

# define	SEM_PHILO_PREFIX	"/philo_"

typedef struct s_table t_table;

typedef struct	s_philo
{
	sem_t			*lock;
	t_table			*table;
	uint64_t		index;
	pthread_t		tid;
	t_time			time_die;
	uint64_t		times_ate;
}				t_philo;

/*
**		philo_actions.c
*/
bool	philo_eat(t_philo *philo);
bool	philo_sleep(t_philo *philo);
bool	philo_think(t_philo *philo);

/*
**		philo.c
*/
void	philo_log(t_philo *philo, const char *message);
void	*philo_thread(void *data);
bool	philo_new(t_philo *philo, t_table *table, uint64_t index);
void	philo_del(t_philo *philo);

#endif
