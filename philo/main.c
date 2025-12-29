/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/12/29 18:22:54 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// all the libs needed for the required funcs
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memset
#include <sys/types.h>
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
	u_int64_t deadline;
	int times_eaten;
	
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;

	pthread_mutex_t *death_check;
	u_int64_t start;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
	pthread_mutex_t *forks;
	pthread_mutex_t death_check;
};
static uint64_t get_time(uint64_t start)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
	uint64_t time = (uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000);
	// printf("[timestamp: %lu]\n", time);
    return ((uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000) - start);
}

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}
static void init_a_philo(struct philosopher *philo, struct sim_info *info, int i)
{
	philo->state = THINK;
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
	philo->death_check = &(info->death_check);
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
	{
		pthread_mutex_init(&(info->forks[i]), NULL);
		i++;
	}
	pthread_mutex_init(&(info->death_check), NULL);
	i = 0;
	while (i < info->num)
	{
		init_a_philo(&((*philosophers)[i]), info, i);
		i++;
	}
	
	// printf("[SIMULATION INFO]\n");
	// printf("Number of philosophers: %d\n", info->num);
	// printf("Time to die: %d ms\n", info->time_to_die);
	// printf("Time to sleep: %d ms\n", info->time_to_sleep);
	// printf("Time to eat: %d ms\n", info->time_to_eat);
	// printf("-----------------------\n");
	return FUNC_SUCCESS;
}

void *philosophise(void *args)
{
	struct philosopher *philo;
	philo = (struct philosopher *)args;
	philo->state = THINK;
	usleep(philo->id % 2 * 50);
	while (1)
	{
		if (philo->id % 2)
		{
			pthread_mutex_lock(philo->fork_left);
			printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
			pthread_mutex_lock(philo->fork_right);
		}
		else
		{
			pthread_mutex_lock(philo->fork_right);
			printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
			pthread_mutex_lock(philo->fork_left);
		}
		// i eat.
		printf("%lu %d is eating\n", get_time(philo->start), philo->id);
		philo->state = EAT;
		// extend deadline
		if (philo->deadline < get_time(philo->start))
		{
			philo->state = DEAD;
			break ;
		}
		else
			philo->deadline = get_time(philo->start) + philo->time_to_die;
		usleep(philo->time_to_eat * 1000);
		philo->times_eaten += 1;
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);

		// i sleep.
		printf("%lu %d is sleeping\n", get_time(philo->start), philo->id);
		philo->state = SLEEP;
		usleep(philo->time_to_sleep * 1000);

		// i check for death.
		pthread_mutex_lock(philo->death_check);
		pthread_mutex_unlock(philo->death_check);
	
		// i think.
		printf("%lu %d is thinking\n", get_time(philo->start), philo->id);
		philo->state = THINK;
		// if sleep is shorter than eat time, then force an idle to ensure we do not get another collision
		if (philo->time_to_sleep < philo->time_to_eat)
			usleep((philo->time_to_eat - philo->time_to_sleep) * 1000);
		// mandatory sleep time
		usleep(100);
	}
	return NULL;
}

int main(int argc, char **argv) 
{
	struct sim_info info;
	struct philosopher *philosophers;
	int death;
	uint64_t start;


	death = 0;
	// printf("argc: %d\n", argc);
	if (argc < 5 || argc > 6)
		return argc_fail();
	if (!init_setup(&info, &philosophers, argc, argv))
	{
		return (EXIT_FAILURE);
	}
	start = get_time(0);
	int i = 0;
	while (i < info.num)
	{
		philosophers[i].id = i + 1;
		philosophers[i].start = start;
		philosophers[i].deadline = info.time_to_die;
		pthread_create(&(philosophers[i].thread), NULL, philosophise, &(philosophers[i]));
		pthread_detach(philosophers[i].thread);
		i++;
	}
	// logic start
	while (1)
	{
		i = 0;
		while (i < info.num)
		{
			if (philosophers[i].state == DEAD || philosophers[i].deadline < get_time(philosophers[i].start))
			{
				printf("%lu %d died\n", get_time(start), philosophers[i].id);
				death = 1;
				break;
			}
			i++;
		}
		if (death)
			break;
		if (info.minimum_eats > -1)
		{
			i = 0;
			while (i < info.num)
			{
				if (philosophers[i].times_eaten < info.minimum_eats)
					break ;
				if (i == info.num - 1)
					death = 1;
				i++;
			}
			if (death)
				break;
		}
	}
	// logic end
	// cleanup
	if (death)
		pthread_mutex_lock(&(info.death_check));
	free(philosophers);
	pthread_mutex_destroy(&(info.death_check));
	// the freeing process needs to be more elaborate here
	return (EXIT_SUCCESS);
}
