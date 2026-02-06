/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 16:48:53 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 20:31:17 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_isspace(int c)
{
	if (c == ' ')
		return (1);
	if (c == '\t')
		return (1);
	if (c == '\r')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\v')
		return (1);
	if (c == '\f')
		return (1);
	return (0);
}

int	ft_atol(const char *nptr)
{
	long	result;
	int		sign;
	char	*s;

	result = 0;
	sign = 1;
	s = (char *)nptr;
	while (*s && ft_isspace(*s))
		s++;
	if (s && (*s == '+' || *s == '-'))
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s && ft_isdigit(*s))
	{
		result *= 10;
		result += (*s - '0');
		s++;
	}
	if (result > INT_MAX)
		result = 0;
	return (result *= sign);
}
