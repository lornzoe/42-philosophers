/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 20:13:29 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

int	validate_args(t_sim *info, int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) > 10)
			return (FUNC_FAIL);
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (!ft_isdigit(argv[i][j]))
				return (FUNC_FAIL);
			j++;
		}
		i++;
	}
	if (info->num <= 0 || info->time_to_die <= 0
		|| info->time_to_eat <= 0 || info->time_to_sleep <= 0)
		return (FUNC_FAIL);
	if (argc == 6 && info->minimum_eats <= 0)
		return (FUNC_FAIL);
	return (FUNC_SUCCESS);
}

static int	init_sems(t_sim *info)
{
	int	seats;

	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_SEATS);
	seats = info->num / 2;
	if (seats < 1)
		seats = 1;
	info->forks = sem_open(SEM_FORKS, O_CREAT, 0644, info->num);
	info->print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	info->seats = sem_open(SEM_SEATS, O_CREAT, 0644, seats);
	if (info->forks == SEM_FAILED || info->print == SEM_FAILED
		|| info->seats == SEM_FAILED)
		return (FUNC_FAIL);
	return (FUNC_SUCCESS);
}

int	init_info(t_sim *info, int argc, char **argv)
{
	info->pids = NULL;
	info->forks = NULL;
	info->print = NULL;
	info->seats = NULL;
	info->start = 0;
	info->minimum_eats = -1;
	info->num = ft_atol(argv[1]);
	info->time_to_die = ft_atol(argv[2]);
	info->time_to_eat = ft_atol(argv[3]);
	info->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		info->minimum_eats = ft_atol(argv[5]);
	if (!validate_args(info, argc, argv))
		return (FUNC_FAIL);
	info->pids = malloc(sizeof(pid_t) * info->num);
	if (!info->pids)
		return (FUNC_FAIL);
	memset(info->pids, 0, sizeof(pid_t) * info->num);
	return (init_sems(info));
}
