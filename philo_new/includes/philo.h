/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/07 18:45:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <string.h>
# include <pthread.h>

# include <utils.h>
# include <table.h>

typedef enum	e_philo_state
{
	TAKING_FORK, EATING, SLEEPING, THINKING, STARVED
}				t_philo_state;

static const char	*g_state_msgs[] =
{
	"is taking a fork", "is eating", "is sleeping", "is thinking", "has died"
};

typedef struct	s_philo
{
	t_uint			forks[2];
	pthread_mutex_t	lock_time_starve;
	char			log_buffer[32];
	t_uint			index;
	t_time			time_starve;
	pthread_t		tid_observer;
}				t_philo;

/*
**				philo_actions.c
*/
//bool			philo_eat(t_philo *philo);
//bool			philo_sleep(t_philo *philo, t_time duration,
//	const char *message, size_t message_size);
//bool			philo_think(t_philo *philo);

bool			philo_set(t_philo *philo, t_uint index);
bool			philo_log(t_time now, t_philo *philo, t_philo_state state);
void			*philo_thread(void *data);

#endif
