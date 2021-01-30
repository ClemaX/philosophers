#include <utils.h>

t_time	time_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (t_time)1000 + now.tv_usec / 1000);
}

void	sleep_until(t_time then)
{
	while (time_millis() < then)
		usleep(100);
}
