/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 23:55:53 by chamada           #+#    #+#             */
/*   Updated: 2020/12/29 23:55:53 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

int	main(int ac, const char **av)
{
	t_philo	*philos;
	bool	success;

	success = false;
	if (!table_new(&philos, ac, av))
		return (1);
	if (table_start(philos))
		success = table_join(philos);
	table_del(&philos);
	return (success ? 0 : -1);
}
