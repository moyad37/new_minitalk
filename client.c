/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmanssou <mmanssou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 17:09:07 by mmanssou          #+#    #+#             */
/*   Updated: 2023/06/26 14:49:48 by mmanssou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*convert_char_to_binary(char c)
{
	int		dec;
	int		i;
	char	*binary;
	int		bit;

	i = 7;
	dec = (int)c;
	binary = malloc(sizeof(char) * 9);
	if (binary == NULL)
	{
		write(1, "memor y error\n", 14);
		exit(1);
	}
	while (i >= 0)
	{
		bit = (dec >> i) & 1;
		binary[7 - i] = bit + '0';
		i--;
	}
	binary[8] = '\0';
	return (binary);
}

void	send_binary(char *binary, pid_t pid)
{
	int	i;

	i = 0;
	while (i <= 7)
	{
		if (binary[i] == '0')
		{
			kill(pid, SIGUSR1);
			usleep(300);
		}
		else if (binary[i] == '1')
		{
			kill(pid, SIGUSR2);
			usleep(300);
		}
		i++;
	}
}

void	send_null(pid_t pid)
{
	int	i;

	i = 0;
	while (i != 8)
	{
		kill(pid, SIGUSR1);
		usleep(300);
		i++;
	}
}

void	handle_av(const char *av, pid_t pid)
{
	int		i;
	char	*binary;

	i = 0;
	while (av[i] != '\0')
	{
		binary = convert_char_to_binary(av[i]);
		send_binary(binary, pid);
		free(binary);
		i++;
	}
	send_null(pid);
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		write(2, "ERROR\n", 6);
		return (0);
	}
	else
	{
		if (!*av[2])
			return (0);
		else
		{
			handle_av(av[2], ft_atoi(av[1]));
		}
	}
	return (0);
}
