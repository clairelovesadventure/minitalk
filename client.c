#include "libft/libft.h"
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t	g_received;

void	confirm_handler(int signum)
{
	(void)signum;
	g_received = 1;
}

void	send_char(int pid, unsigned char c)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		g_received = 0;
		if (c & (0x80 >> bit))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!g_received)
			usleep(100);
		bit++;
	}
}

int	main(int argc, char **argv)
{
	int		pid;
	char	*str;
	int		i;

	if (argc != 3)
	{
		ft_printf("Usage: %s <pid> <string>\n", argv[0]);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		ft_printf("Error: Invalid PID\n");
		return (1);
	}
	str = argv[2];
	signal(SIGUSR1, confirm_handler);
	i = 0;
	while (str[i])
	{
		send_char(pid, (unsigned char)str[i]);
		i++;
	}
	send_char(pid, '\0');
	return (0);
}
