#ifndef UTILS_H
# define UTILS_H

# include <sys/time.h>

# include <unistd.h>
# include <stdint.h>

typedef	uint64_t	t_time;

t_time		clock_millis(void);

uint64_t	atoui(const char *str);
const char	*uitoa(uint64_t number);
int			putui(int fd, uint64_t number);

size_t		ft_strlen(const char *str);

#endif
