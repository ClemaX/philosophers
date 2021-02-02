/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 02:05:35 by chamada           #+#    #+#             */
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
	pthread_t		tid;
	pthread_t		tid_observer;
	t_time			time_die;
	t_uint			times_ate;
}				t_philo;

/*
**				philo_actions.c
*/
t_time			philo_eat(t_philo *philo);
t_time			philo_sleep(t_philo *philo);
t_time			philo_think(t_philo *philo);

/*
**				philo.c
*/
void			*philo_thread(void *data);
bool			philo_set(t_philo *philo, t_uint index);
void			philo_del(t_philo *philo);

#endif
