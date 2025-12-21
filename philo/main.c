/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/12/21 20:09:10 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// all the libs needed for the required funcs
#include <bits/pthreadtypes.h>
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
	pthread_t thread;
	int id;
	
	int state;
	int time_in_state;
	int deadline;
	int times_eaten;
	
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
	pthread_mutex_t *forks;
};

static int dt(struct philosopher *philo)
{
	// get delta time with this
	return 0;
}

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}
static void init_a_philo(struct philosopher *philo, int time_to_die, struct sim_info *info, int i)
{
	philo->state = THINK;
	philo->deadline = time_to_die;
	philo->time_in_state = 0;
	philo->times_eaten = 0;

	philo->time_to_die = info->time_to_die;
	philo->time_to_eat = info->time_to_eat;
	philo->time_to_sleep = info->time_to_sleep;

	// fork-setting
	philo->fork_left = &(info->forks[i]);
	if (i + 1 == info->num)
		philo->fork_right = &(info->forks[0]);
	else
	 	philo->fork_right = &(info->forks[i + 1]);
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
	info->forks = malloc(sizeof(pthread_mutex_t) * (info->num + 1));
	if (!(*philosophers) || !info->forks)
		return FUNC_FAIL;
	i = 0;
	while (i < info->num)
		pthread_mutex_init(&(info->forks[i]), NULL);
	i = 0;
	while (i < info->num)
		init_a_philo(philosophers[i], info->time_to_die, info, i++);
	
	printf("[SIMULATION INFO]\n");
	printf("Number of philosophers: %d\n", info->num);
	printf("Time to die: %d seconds\n", info->time_to_die);
	printf("Time to sleep: %d seconds\n", info->time_to_sleep);
	printf("Time to eat: %d seconds\n", info->time_to_eat);
	printf("-----------------------\n");
	return FUNC_SUCCESS;
}

void *philosophise(void *args)
{
	struct philosopher philo;
	philo = *((struct philosopher *)args);
	while (1)
	{
		philo.state = THINK;
		pthread_mutex_lock(philo.fork_left);
		pthread_mutex_lock(philo.fork_right);
		
		philo.state = EAT;
		usleep(philo.time_to_eat * 1000000);
		pthread_mutex_unlock(philo.fork_left);
		pthread_mutex_unlock(philo.fork_right);
		// extend deadline here
		philo.state = SLEEP;
		usleep(philo.time_to_sleep * 1000000);

	}
	return NULL;
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
	// ready set go!
	int i = 0;
	while (i < info.num)
		philosophers[i].id = pthread_create(&(philosophers[i].thread), NULL, philosophise, &(philosophers[i]));
	// logic start

	// logic end
	// cleanup
	free(philosophers);
	return (EXIT_SUCCESS);
}
