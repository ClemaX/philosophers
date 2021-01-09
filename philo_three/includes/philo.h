/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 18:03:08 by chamada           #+#    #+#             */
/*   Updated: 2021/01/07 18:45:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>

# include <utils.h>
# include <table.h>

# define SEM_PHILO_PREFIX	"/philo_"

typedef struct	s_philo
{
	sem_t			*lock;
	t_uint			index;
	pid_t			pid;
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

/*
**				philo.c
*/
bool			philo_start(t_philo *philo);
void			*philo_thread(void *data);
bool			philo_set(t_philo *philo, t_uint index);
void			philo_del(t_philo *philo);

/*
**				observer.c
*/
void			*observer_thread(void *data);
bool			observer_start(t_philo *philo);

#endif
