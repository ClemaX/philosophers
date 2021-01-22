/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 16:51:37 by chamada           #+#    #+#             */
/*   Updated: 2021/01/07 18:45:07 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <sys/time.h>

# include <unistd.h>
# include <stdint.h>
# include <errno.h>

# ifndef UINT_PADDING
#  define UINT_PADDING "00000000"
# endif

typedef	uint64_t	t_uint;
typedef	t_uint		t_time;

typedef struct		s_errmap
{
	int		err_code;
	char	*err_msg;
}					t_errmap;

/*
**					utils_time.c
*/
t_time				time_millis(void);
void				sleep_until(t_time time);

/*
**					utils_int.c
*/
unsigned char		uilen(t_uint number);
t_uint				atoui(const char *str);
const char			*uitoa(t_uint number, unsigned char *len);
int					putui(int fd, t_uint number, unsigned char field_width);
void				strputui(char *dest, t_uint number,
	unsigned char field_width);

/*
**					utils_str.c
*/
size_t				ft_strlen(const char *str);
const char			*ft_strerror(int err);

#endif
