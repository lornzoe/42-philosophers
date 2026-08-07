/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 20:14:40 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo->deadline < get_time(philo->info->start))
		{
			log_death(philo);
			child_exit(philo->info, EXIT_FAILURE);
		}
		usleep(500);
	}
	return (NULL);
}

void	kill_all(t_sim *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		if (info->pids[i] > 0)
			kill(info->pids[i], SIGKILL);
		i++;
	}
}

void	reaper(t_sim *info)
{
	int	finished;
	int	status;

	finished = 0;
	while (finished < info->num)
	{
		if (waitpid(-1, &status, 0) < 0)
			break ;
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
		{
			kill_all(info);
			break ;
		}
		finished++;
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
}
