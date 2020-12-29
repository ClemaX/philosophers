#include <utils.h>

size_t	ft_strlen(const char *str)
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
		{ESRCH, "Invalid thread/process id!"},
		{EAGAIN, "Insufficient resources!"},
		{ENOMEM, "Insufficient memory!"},
		{EINVAL, "Invalid argument!"},
		{EDEADLK, "A deadlock has been detected!"},
	};
	unsigned char				i;

	i = 0;
	while (i < sizeof(errmap) / sizeof(*errmap) && errmap[i].err_code != err)
		i++;
	return (i == sizeof(errmap) / sizeof(*errmap) ? NULL : errmap->err_msg);
}
