/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2026/01/10 07:46:00 by lyanga           ###   ########.fr       */
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

struct philosopher 
{
	pthread_t thread;
	int id;
	
	int alive;
	int time_in_state;
	u_int64_t deadline;
	int times_eaten;
	
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int forks[2];
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;

	u_int64_t start;

	int *sim_death;
	int thread_end;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
	pthread_mutex_t *forks;
	int death;
};

static uint64_t get_time(uint64_t start)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000) - start);
}

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}
static void init_a_philo(struct philosopher *philo, struct sim_info *info, int i)
{
	philo->alive = 1;
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
	philo->id = i + 1;
	philo->deadline = info->time_to_die;
	philo->sim_death = &(info->death);
	philo->forks[0] = 0;
	philo->forks[1] = 0;
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


void sleep_for(int sleepeatdiff, uint64_t timeleft, uint64_t percentage)
{
    if (sleepeatdiff > 0)
	{
		usleep((uint64_t)sleepeatdiff * 1000);
		if (timeleft > (uint64_t)sleepeatdiff)
			timeleft -= (uint64_t)sleepeatdiff;
	}
	if (timeleft > 0)
    	usleep(timeleft * 10 * percentage);
}

uint64_t positive_diff(uint64_t left, uint64_t right)
{
	if (left < right)
		return (0);
	return (left - right);
}

void *philosophise(void *args)
{
	struct philosopher *philo;
	philo = (struct philosopher *)args;

	// absolute death case -- 1 philosopher.
	if (philo->fork_left == philo->fork_right)
	{
		pthread_mutex_lock(philo->fork_left);
		printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
		while (!*(philo->sim_death))
			continue;
		pthread_mutex_unlock(philo->fork_left);
		philo->thread_end = 1;
		return NULL;
	}
	usleep(philo->id % 2 * 250);
	while (1)
	{
		if (*(philo->sim_death))
			break;
		if (philo->id % 2)
		{
			pthread_mutex_lock(philo->fork_left);
			philo->forks[0] = 1;
			if (*(philo->sim_death))
				break;
			printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
			pthread_mutex_lock(philo->fork_right);
			philo->forks[1] = 1;
		}
		else
		{
			pthread_mutex_lock(philo->fork_right);
			philo->forks[1] = 1;
			if (*(philo->sim_death))
				break;
			printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
			pthread_mutex_lock(philo->fork_left);
			philo->forks[0] = 1;
		}
		if (*(philo->sim_death))
			break;
		printf("%lu %d has taken a fork\n", get_time(philo->start), philo->id);
		printf("%lu %d is eating\n", get_time(philo->start), philo->id);
		// extend deadline
		if (philo->deadline < get_time(philo->start))
		{
			philo->alive = 0;
			break ;
		}
		else
			philo->deadline = get_time(philo->start) + philo->time_to_die;
		usleep(philo->time_to_eat * 1000);
		philo->times_eaten += 1;
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		philo->forks[0] = 0;
		philo->forks[1] = 0;
		// i sleep.
		if (*(philo->sim_death))
			break;
		printf("%lu %d is sleeping\n", get_time(philo->start), philo->id);
		usleep(philo->time_to_sleep * 1000);
	
		// i think.
		if (*(philo->sim_death))
			break;
		printf("%lu %d is thinking\n", get_time(philo->start), philo->id);
		sleep_for(philo->time_to_eat - philo->time_to_sleep,
			positive_diff(philo->deadline, get_time(philo->start)),
			10);
	}
	if (philo->forks[0])
		pthread_mutex_unlock(philo->fork_left);
	if (philo->forks[1])
		pthread_mutex_unlock(philo->fork_right);
	philo->thread_end = 1;
	return NULL;
}

int main(int argc, char **argv) 
{
	struct sim_info info;
	struct philosopher *philosophers;
	uint64_t start;
	int philo_died;

// temp
	int offender_id;

	offender_id = -1;
	info.death = 0;
	philo_died = 0;
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
		philosophers[i].thread_end = 0;
		philosophers[i].start = start;
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
			if (!philosophers[i].alive || philosophers[i].deadline < get_time(philosophers[i].start))
			{
				offender_id = philosophers[i].id;
				info.death = 1;
				philo_died = 1;
				break;
			}
			i++;
		}
		if (info.death)
			break;
		if (info.minimum_eats > -1)
		{
			i = 0;
			while (i < info.num)
			{
				if (philosophers[i].times_eaten < info.minimum_eats)
					break ;
				if (i == info.num - 1)
					info.death = 1;
				i++;
			}
			if (info.death)
				break;
		}
	}
	// logic end

	// cleanup
	// wait for all the philo threads to end before proceeding
	i = 0;
	while (i < info.num)
	{
		if (philosophers[i].thread_end)
			i++;

	}
	free(philosophers);
	// the freeing process needs to be more elaborate here
	if (philo_died)
	{
		printf("%lu %d died\n", get_time(start), offender_id);
	}
	return (EXIT_SUCCESS);
}
