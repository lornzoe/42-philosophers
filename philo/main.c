/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 15:55:24 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}

int main(int argc, char **argv)
{
	t_sim info;
	t_philosopher *philosophers;
	uint64_t start;
	int i;

	i = 0;
	info.death = 0;
	start = 0;
	if (argc < 5 || argc > 6)
		return argc_fail();
	if (!init_setup(&info, &philosophers, argc, argv))
		return (EXIT_FAILURE);
	pthread_mutex_lock(info.race_gate);
	while (i < info.num)
	{
		philosophers[i].start = &start;
		pthread_create(&(philosophers[i].thread), NULL, philosophise, &(philosophers[i]));
		i++;
	}
	start = get_time(0);
	pthread_mutex_unlock(info.race_gate);
	monitoring(&info, philosophers, start);

	// cleanup
	// wait for all the philo threads to end before proceeding
	//printf("DEBUG: SIMULATION HAS ENDED, DOING CLEANUP NOW...\n");
	i = 0;
	
	while (i < info.num)
	{
		pthread_join(philosophers[i].thread, NULL);
		i++;
	}
	free(philosophers);
	// the freeing process needs to be more elaborate here

	return (EXIT_SUCCESS);
}
