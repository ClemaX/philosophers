#include <utils.h>

t_time	time_millis(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * (t_time)1000 + now.tv_usec / 1000);
}

void	sleep_until(t_time time)
{
	const t_time	now = time_millis();

	if (now < time)
		usleep((time - now) * 1000);
}