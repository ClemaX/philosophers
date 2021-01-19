/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 14:39:28 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	*philo_thread(void *data)
{
	t_philo	*const philo = data;

	while (philo_eat(philo)
	&& philo_sleep(philo, g_table.time_to_sleep, "is sleeping")
	&& philo_think(philo))
		;
	return (data);
}

bool	philo_set(t_philo *philo, t_uint index)
{
	const t_uint	wrapped_fork = (index + 1) % (g_table.seats);
	int				err;

	if (!(err = pthread_mutex_init(&philo->lock, NULL)))
	{
		if (wrapped_fork == 0)
		{
			philo->forks[0] = wrapped_fork;
			philo->forks[1] = index;
		}
		else
		{
			philo->forks[0] = index;
			philo->forks[1] = wrapped_fork;
		}
		philo->index = index;
		philo->times_ate = 0;
		philo->time_die = 0;

		dprintf(2, "philo %llu using forks %llu and %llu!\n", index + 1, philo->forks[0], philo->forks[1]);
	}
	return (!err);
}
