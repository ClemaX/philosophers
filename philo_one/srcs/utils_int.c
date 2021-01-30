/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 13:09:08 by chamada           #+#    #+#             */
/*   Updated: 2021/01/30 13:17:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

unsigned char	uilen(t_uint number)
{
	unsigned char	len;

	len = 1;
	while (number /= 10)
		len++;
	return (len);
}

const char		*uitoa(t_uint number, unsigned char *len_dest)
{
	static char		str[21];
	unsigned char	len;

	len = uilen(number);
	if (len_dest)
		*len_dest = len;
	str[len] = '\0';
	while (len--)
	{
		str[len] = number % 10 + '0';
		number /= 10;
	}
	return (str);
}

int				putui(int fd, t_uint number, unsigned char fw)
{
	static const int	pad_max = sizeof(UINT_PADDING) - 1;
	unsigned char		len;
	const char *const	str = uitoa(number, &len);
	const int			pad_len = fw <= pad_max ? fw - len : pad_max - len;

	if (fw && write(fd, UINT_PADDING, pad_len) == -1)
		return (-1);
	return (write(fd, str, len));
}

void			strputui(char *dest, t_uint number, unsigned char fw)
{
	static const int	pad_max = sizeof(UINT_PADDING) - 1;
	unsigned char		num_len;
	int					pad_len;

	num_len = uilen(number);
	pad_len = fw <= pad_max ? fw - num_len : pad_max - num_len;
	if (pad_len >= 0)
	{
		num_len += pad_len;
		while (num_len-- > pad_len)
		{
			dest[num_len] = number % 10 + '0';
			number /= 10;
		}
	}
}

t_uint			atoui(const char *str)
{
	t_uint	i;

	i = 0;
	while (*str >= '0' && *str <= '9' && i < INT64_MAX)
		i = i * 10 + *str++ - '0';
	return ((*str || i >= INT64_MAX) ? 0 : i);
}
