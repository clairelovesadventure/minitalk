/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_username <your_email>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by your_username     #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by your_username    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <signal.h>
#include <unistd.h>

static void handle_signal(int sig, siginfo_t *info, void *context)
{
    static int              received_bits = 0;
    static unsigned char    current_byte = 0;
    
    (void)context;

    // 左移一位，为新的位腾出空间
    current_byte = current_byte << 1;
    // 如果收到SIGUSR1，设置最后一位为1
    if (sig == SIGUSR1)
        current_byte = current_byte | 1;
    received_bits++;

    // 发送确认信号给客户端
    kill(info->si_pid, SIGUSR1);

    // 收集够8位后处理
    if (received_bits == 8)
    {
        if (current_byte == '\0')
            ft_printf("\n");
        else
            write(1, &current_byte, 1);
        received_bits = 0;
        current_byte = 0;
    }
}

int main(void)
{
    struct sigaction sa;

    ft_printf("Server PID: %d\n", getpid());
    
    // 设置sigaction结构体
    sa.sa_sigaction = handle_signal;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    // 注册信号处理函数
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || 
        sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        ft_printf("Error: Failed to set signal handlers\n");
        return (1);
    }

    while (1)
        pause();
    return (0);
} 