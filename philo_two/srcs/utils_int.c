#include <utils.h>

static unsigned char	uilen(t_uint number)
{
	unsigned char	len;

	len = 1;
	while (number /= 10)
		len++;
	return (len);
}

const char				*uitoa(t_uint number, unsigned char *len_dest)
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

int						putui(int fd, t_uint number)
{
	unsigned char		len;
	const char *const	str = uitoa(number, &len);

	return (write(fd, str, len));
}

t_uint					atoui(const char *str)
{
	t_uint	i;

	i = 0;
	while (*str >= '0' && *str <= '9' && i < INT64_MAX)
		i = i * 10 + *str++ - '0';
	return ((i >= INT64_MAX) ? 0 : i);
}