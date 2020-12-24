#include <utils.h>

t_time			clock_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (t_time)1000 + now.tv_usec / 1000);
}
