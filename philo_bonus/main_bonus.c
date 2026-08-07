/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 20:13:22 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	argc_fail(void)
{
	printf("usage: number_of_philosophers \
time_to_die time_to_eat time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n");
	return (EXIT_FAILURE);
}

static void	cleanup(t_sim *info)
{
	if (info->forks && info->forks != SEM_FAILED)
		sem_close(info->forks);
	if (info->print && info->print != SEM_FAILED)
		sem_close(info->print);
	if (info->seats && info->seats != SEM_FAILED)
		sem_close(info->seats);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_SEATS);
	if (info->pids)
		free(info->pids);
}


static int	spawn_children(t_sim *info)
{
	int	i;

	i = 0;
	info->start = get_time(0);
	while (i < info->num)
	{
		info->pids[i] = fork();
		if (info->pids[i] < 0)
			return (FUNC_FAIL);
		if (info->pids[i] == 0)
			child_process(info, i + 1);
		i++;
	}
	return (FUNC_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_sim	info;

	if (argc < 5 || argc > 6)
		return (argc_fail());
	if (!init_info(&info, argc, argv))
	{
		cleanup(&info);
		return (EXIT_FAILURE);
	}
	if (!spawn_children(&info))
		kill_all(&info);
	reaper(&info);
	cleanup(&info);
	return (EXIT_SUCCESS);
}
