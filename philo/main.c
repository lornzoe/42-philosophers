/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/12/21 03:58:15 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// all the libs needed for the required funcs
#include <inttypes.h>
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memset
#include <unistd.h> // write, usleep
#include <sys/time.h> // gettimeofday
#include <pthread.h> // pthread funcs (3) + pthread_mutex funcs (4)

#define FUNC_SUCCESS 1
#define FUNC_FAIL 0

enum philostate
{
	DEAD = 0,
	THINK,
	EAT,
	SLEEP
};

struct philosopher 
{
	int state;
	int time_in_state;
	int deadline;
	int times_eaten;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
};

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}
static void init_a_philo(struct philosopher *philo, int time_to_die)
{
	philo->state = THINK;
	philo->deadline = time_to_die;
	philo->time_in_state = 0;
	philo->times_eaten = 0;
}

static int init_setup(struct sim_info *info, struct philosopher **philosophers, int argc, char **argv)
{
	int i;

	// need to do: sanitise numbers.
	info->num = atoi(argv[1]);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		info->minimum_eats = atoi(argv[5]);
	else
	 	info->minimum_eats = -1;
	*philosophers = malloc(sizeof(struct philosopher) * (info->num + 1));
	if (!(*philosophers))
		return FUNC_FAIL;
	i = 0;
	while (i < info->num)
		init_a_philo(philosophers[i++], info->time_to_die);
	
	printf("[SIMULATION INFO]\n");
	printf("Number of philosophers: %d\n", info->num);
	printf("Time to die: %d seconds\n", info->time_to_die);
	printf("Time to sleep: %d seconds\n", info->time_to_sleep);
	printf("Time to eat: %d seconds\n", info->time_to_eat);
	printf("-----------------------\n");
	return FUNC_SUCCESS;
}

int main(int argc, char **argv) 
{
	struct sim_info info;
	struct philosopher *philosophers;	
	printf("argc: %d\n", argc);
	if (argc < 5 || argc > 6)
		return argc_fail();
	if (!init_setup(&info, &philosophers, argc, argv))
		return (EXIT_FAILURE);
	// logic start

	// logic end
	// cleanup
	free(philosophers);
	return (EXIT_SUCCESS);
}
