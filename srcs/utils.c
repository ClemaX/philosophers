#include <utils.h>

t_time			clock_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (t_time)1000 + now.tv_usec / 1000);
}


uint64_t		atoui(const char *str)
{
	uint64_t	i;

	i = 0;
	while (*str >= '0' && *str <= '9' && i < INT64_MAX)
		i = i * 10 + *str++ - '0';
	return ((i >= INT64_MAX) ? 0 : i);
}

unsigned char	uilen(uint64_t number)
{
	unsigned char	len;

	len = 1;
	while (number /= 10)
		len++;
	return (len);
}

int				putui(int fd, uint64_t number)
{
	static char			str[20];
	const unsigned char	len = uilen(number);
	unsigned char		i;

	i = len;
	str[i] = '\0';
	while (i--)
	{
		str[i] = number % 10 + '0';
		number /= 10;
	}
	return (write(fd, str, len));
}

size_t			ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}
