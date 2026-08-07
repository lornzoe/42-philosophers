/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/09 16:09:30 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	argc_fail(void)
{
	printf("usage: number_of_philosophers \
time_to_die time_to_eat time_to_sleep \
[number_of_times_each_philosopher_must_eat]\n");
	return (EXIT_FAILURE);
}

static void	start_sim(t_sim *info, t_philosopher *philosophers)
{
	int	i;

	i = 0;
	info->death = 0;
	info->start = get_time(0);
	while (i < info->num)
	{
		philosophers[i].start = info->start;
		pthread_create(&(philosophers[i].thread),
			NULL, philosophise, &(philosophers[i]));
		i++;
	}
	monitoring(info, philosophers, info->start);
	i = 0;
	while (i < info->num)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
}

static void	cleanup_mutexs(t_sim *info)
{
	int	i;

	i = 0;
	while (i < info->num)
	{
		if (info->forks)
			pthread_mutex_destroy(&(info->forks[i]));
		i++;
	}
	if (info->print_lock)
		pthread_mutex_destroy(info->print_lock);
}

static void	cleanup_sim(t_sim *info)
{
	cleanup_mutexs(info);
	if (info->forks)
		free(info->forks);
	if (info->print_lock)
		free(info->print_lock);
}

int	main(int argc, char **argv)
{
	t_sim			info;
	t_philosopher	*philosophers;

	if (argc < 5 || argc > 6)
		return (argc_fail());
	philosophers = NULL;
	if (!init_setup(&info, &philosophers, argc, argv))
	{
		if (validate_args(&info, argc, argv))
			cleanup_sim(&info);
		if (philosophers)
			free(philosophers);
		return (EXIT_FAILURE);
	}
	start_sim(&info, philosophers);
	cleanup_sim(&info);
	free(philosophers);
	return (EXIT_SUCCESS);
}
