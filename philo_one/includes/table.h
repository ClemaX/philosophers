/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/07 18:50:14 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_H
# define TABLE_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

# include <utils.h>
# include <philo.h>
# include <observer.h>

# ifndef NAME_MAX
#  define NAME_MAX 255
# endif

# ifndef MSG_DELIM
#  define MSG_DELIM	" "
# endif

# define MSG_EUSAGE_PREFIX	"Usage: "
# define MSG_EUSAGE_ARGS	" seats time_to_die time_to_eat time_to_sleep"
# define MSG_EUSAGE_OPTS	" [appetite]"
# define MSG_EUSAGE_SUFFIX	"\nThe number of seats must be greater than 1.\n"

# define MSG_DIED		"died"
# define MSG_EATING		"is eating"
# define MSG_THINKING	"is thinking"
# define MSG_SLEEPING	"is sleeping"
# define MSG_TAKE_FORK	"has taken a fork"

/*
**	                     <---8--> <3> <------16------>
*/
# define MSG_TEMPLATE	"00000000 000                 \n"

# define MSG_TSLEN		8U
# define MSG_IDXLEN		3U
# define MSG_MAXLEN		16U


typedef struct			s_table
{
	bool			running;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock_write;
	pthread_mutex_t	lock_run;
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

typedef struct s_philo	t_philo;

void					table_show_usage(const char *name);
void					table_perror(const char *msg, int err);
t_time					table_log(t_philo *philo, const char *message,
	size_t size);

bool					table_running(void);
bool					table_running_log(t_philo *philo, const char *message,
	size_t length);

bool					table_new(t_philo **philos, int ac, const char **av);
void					table_del(t_philo **philos);

bool					table_start(t_philo *philos);
bool					table_join(t_philo *philos);

#endif
