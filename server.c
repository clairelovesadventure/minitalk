#include "libft/libft.h"
#include <signal.h>
#include <unistd.h>

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	static int				bit;
	static unsigned char	c;
	static int				prev_pid;

	(void)context;
	if (info->si_pid != prev_pid && prev_pid != 0)
	{
		write(1, "\n", 1);
		bit = 0;
		c = 0;
	}
	prev_pid = info->si_pid;
	if (signum == SIGUSR1)
		c |= (0x80 >> bit);
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
			write(1, "\n", 1);
		else
			write(1, &c, 1);
		bit = 0;
		c = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("Server PID: %d\n", getpid());
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		ft_printf("Error: Failed to set signal handlers\n");
		return (1);
	}
	while (1)
		pause();
	return (0);
}
