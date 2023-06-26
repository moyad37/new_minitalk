/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanssou <mmanssou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:08:58 by mmanssou          #+#    #+#             */
/*   Updated: 2023/06/26 14:49:46 by mmanssou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 100000

typedef struct s_save_char
{
	int		bin[8];
	int		c;
	int		counter;
	char	message[MAX];
	int		pid;
	size_t	message_size;
}			t_save_char;

t_save_char	g_my;

void	init_data(t_save_char *g_my)
{
	int	i;

	i = 0;
	g_my->counter = 0;
	g_my->c = 0;
	g_my->message_size = 0;
	g_my->pid = getpid();
	while (i++ < 8)
	{
		g_my->bin[i] = 0;
		i++;
	}
}

int	convert_bin_to_char(int *bin)
{
	int		i;
	int		dec;
	int		base;
	char	ascii;

	base = 1;
	dec = 0;
	i = 7;
	while (i >= 0)
	{
		if (bin[i] != 0 && bin[i] != 1)
			return (-1);
		if (bin[i] == 1)
			dec += base;
		base *= 2;
		i--;
	}
	ascii = (char)dec;
	return (ascii);
}

void	check_complet(int counter)
{
	char	c;

	if (counter != 8)
		return ;
	else if (counter == 8)
	{
		g_my.c = convert_bin_to_char(g_my.bin);
		c = g_my.c;
		if (g_my.message_size + 1 >= MAX)
		{
			write(1, g_my.message, g_my.message_size);
			g_my.message_size = 0;
			ft_memset(g_my.message, 0, sizeof(g_my.message));
		}
		g_my.message[g_my.message_size] = c;
		g_my.message_size++;
		g_my.counter = 0;
		if (c == '\0')
		{
			write(1, g_my.message, g_my.message_size);
			g_my.message_size = 0;
			ft_memset(g_my.message, 0, sizeof(g_my.message));
		}
	}
}

void	handle_code(int seg)
{
	if (seg != SIGUSR1 && seg != SIGUSR2)
		return ;
	if (seg == SIGUSR1)
	{
		g_my.bin[g_my.counter] = 0;
		g_my.counter++;
		usleep(10);
	}
	else if (seg == SIGUSR2)
	{
		g_my.bin[g_my.counter] = 1;
		g_my.counter++;
		usleep(10);
	}
	check_complet(g_my.counter);
}

int	main(void)
{
	struct sigaction	sa;

	init_data(&g_my);
	write(1, "id= ", 4);
	ft_putnbr_fd(g_my.pid, 1);
	write(1, "\n", 1);
	sa.sa_handler = handle_code;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
