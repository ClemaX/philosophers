/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 13:09:08 by chamada           #+#    #+#             */
/*   Updated: 2021/01/09 16:03:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

size_t		ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

const char	*ft_strerror(int err)
{
	static const t_errmap	errmap[] = {
		{EPERM, "Permission denied!"},
		{ENOENT, "No such file or directory!"},
		{ESRCH, "Invalid thread/process id!"},
		{EAGAIN, "Insufficient resources!"},
		{ENOMEM, "Insufficient memory!"},
		{EACCES, "Permission denied!"},
		{EINVAL, "Invalid argument!"},
		{EEXIST, "File already exists!"},
		{ENFILE, "File table overflow!"},
		{EMFILE, "Too many open files!"},
		{EDEADLK, "A deadlock has been detected!"},
		{ENAMETOOLONG, "File name too long!"},
	};
	unsigned char			i;

	i = 0;
	while (i < sizeof(errmap) / sizeof(*errmap) && errmap[i].err_code != err)
		i++;
	return (i == sizeof(errmap) / sizeof(*errmap) ? NULL : errmap->err_msg);
}
