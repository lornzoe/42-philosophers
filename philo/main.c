/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 20:26:04 by lyanga           ###   ########.fr       */
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
	uint64_t	start;
	int			i;

	i = 0;
	start = 0;
	info->death = 0;
	pthread_mutex_lock(info->race_gate);
	while (i < info->num)
	{
		philosophers[i].start = &start;
		pthread_create(&(philosophers[i].thread),
			NULL, philosophise, &(philosophers[i]));
		i++;
	}
	start = get_time(0);
	pthread_mutex_unlock(info->race_gate);
	monitoring(info, philosophers, start);
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
		if (info->deadline_locks)
			pthread_mutex_destroy(&(info->deadline_locks[i]));
		if (info->eaten_locks)
			pthread_mutex_destroy(&(info->eaten_locks[i]));
		i++;
	}
	if (info->simdeath_lock)
		pthread_mutex_destroy(info->simdeath_lock);
	if (info->print_lock)
		pthread_mutex_destroy(info->print_lock);
	if (info->race_gate)
		pthread_mutex_destroy(info->race_gate);
}

static void	cleanup_sim(t_sim *info)
{
	cleanup_mutexs(info);
	if (info->forks)
		free(info->forks);
	if (info->deadline_locks)
		free(info->deadline_locks);
	if (info->eaten_locks)
		free(info->eaten_locks);
	if (info->simdeath_lock)
		free(info->simdeath_lock);
	if (info->print_lock)
		free(info->print_lock);
	if (info->race_gate)
		free(info->race_gate);
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
