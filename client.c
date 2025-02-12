#include "libft/libft.h"
#include <signal.h>
#include <unistd.h>

static int g_signal_received = 0;

// 处理服务器的确认信号
static void signal_handler(int sig)
{
    (void)sig;
    g_signal_received = 1;
}

// 发送一个字节的数据
static void	send_byte(int server_pid, unsigned char byte)
{
    int	bit_position;

    bit_position = 7;
    while (bit_position >= 0)
    {
        g_signal_received = 0;
        if (byte & (1 << bit_position))
            kill(server_pid, SIGUSR1);
        else
            kill(server_pid, SIGUSR2);
        
        // 等待服务器确认
        while (!g_signal_received)
            usleep(100);
        bit_position--;
    }
}

int	main(int argc, char **argv)
{
    int		server_pid;
    char	*msg;
    int		i;

    if (argc != 3)
    {
        ft_printf("Usage: ./client <server_pid> <message>\n");
        return (1);
    }
    
    // 设置信号处理函数
    signal(SIGUSR1, signal_handler);
    
    server_pid = ft_atoi(argv[1]);
    if (server_pid <= 0)
    {
        ft_printf("Error: invalid PID\n");
        return (1);
    }
    msg = argv[2];
    i = 0;
    while (msg[i])
        send_byte(server_pid, msg[i++]);
    send_byte(server_pid, '\0');  // 发送终止符
    return (0);
} 
