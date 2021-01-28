/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 17:05:18 by chamada           #+#    #+#             */
/*   Updated: 2020/12/30 17:05:18 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

t_time	time_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (t_time)1000 + now.tv_usec / 1000);
}

void	sleep_until(t_time time_wake)
{
	while (time_millis() < time_wake)
		usleep(100);
}

