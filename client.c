#include "libft/libft.h"
#include <signal.h>
#include <unistd.h>

typedef struct s_client_data
{
    int signal_received;
} t_client_data;

static void signal_handler(int sig, siginfo_t *info, void *context)
{
    t_client_data *data;

    (void)info;
    (void)context;
    (void)sig;
    data = (t_client_data *)context;
    data->signal_received = 1;
}

static void send_bit(int server_pid, int bit, t_client_data *data)
{
    data->signal_received = 0;
    if (bit)
        kill(server_pid, SIGUSR1);
    else
        kill(server_pid, SIGUSR2);
    while (!data->signal_received)
        usleep(100);
}

static void send_byte(int server_pid, unsigned char byte, t_client_data *data)
{
    int bit_position;

    bit_position = 7;
    while (bit_position >= 0)
    {
        send_bit(server_pid, byte & (1 << bit_position), data);
        bit_position--;
    }
}

static int init_client(t_client_data *data)
{
    struct sigaction sa;

    data->signal_received = 0;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        return (-1);
    return (0);
}

static void send_message(int server_pid, char *msg, t_client_data *data)
{
    int i;

    i = 0;
    while (msg[i])
        send_byte(server_pid, msg[i++], data);
    send_byte(server_pid, '\0', data);
}

int main(int argc, char **argv)
{
    t_client_data data;
    int server_pid;

    if (argc != 3)
    {
        ft_printf("Usage: ./client <server_pid> <message>\n");
        return (1);
    }
    server_pid = ft_atoi(argv[1]);
    if (server_pid <= 0)
    {
        ft_printf("Error: invalid PID\n");
        return (1);
    }
    if (init_client(&data) == -1)
        return (1);
    send_message(server_pid, argv[2], &data);
    return (0);
} 
