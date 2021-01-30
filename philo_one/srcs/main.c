/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/30 13:17:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <philosophers.h>

int	main(int ac, const char **av)
{
	t_philo	*philos;
	bool	success;

	if (!table_new(&philos, ac, av))
		return (1);
	success = false;
	if (table_start(philos))
		success = table_join(philos);
	table_del(&philos);
	return (success ? 0 : -1);
}
