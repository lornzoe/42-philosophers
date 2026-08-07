/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 20:16:22 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	put_num(char *buf, uint64_t n)
{
	uint64_t	tmp;
	int			len;
	int			i;

	len = 1;
	tmp = n;
	while (tmp >= 10)
	{
		tmp /= 10;
		len++;
	}
	i = len;
	while (i > 0)
	{
		i--;
		buf[i] = '0' + (n % 10);
		n /= 10;
	}
	return (len);
}

static int	put_str(char *buf, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		buf[i] = s[i];
		i++;
	}
	return (i);
}

static int	build_line(char *buf, t_philo *philo, char *str)
{
	int	len;

	len = put_num(buf, get_time(philo->info->start));
	buf[len] = ' ';
	len++;
	len += put_num(buf + len, philo->id);
	buf[len] = ' ';
	len++;
	len += put_str(buf + len, str);
	buf[len] = '\n';
	len++;
	return (len);
}

void	log_action(t_philo *philo, char *str)
{
	char	buf[128];
	int		len;

	sem_wait(philo->info->print);
	len = build_line(buf, philo, str);
	write(1, buf, len);
	sem_post(philo->info->print);
}

void	log_death(t_philo *philo)
{
	char	buf[128];
	int		len;

	sem_wait(philo->info->print);
	len = build_line(buf, philo, "died");
	write(1, buf, len);
}
