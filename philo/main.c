/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:59:17 by lyanga            #+#    #+#             */
/*   Updated: 2026/01/12 21:23:47 by lyanga           ###   ########.fr       */
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
	
	u_int64_t deadline;
	int times_eaten;
	
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int forks[2];
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;

	pthread_mutex_t *deadline_lock;
	pthread_mutex_t *eaten_lock;
	pthread_mutex_t *simdeath_lock;
	pthread_mutex_t *print_lock;

	pthread_mutex_t *race_gate;
	
	u_int64_t *start;

	int *sim_death;
	// int thread_end;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
	pthread_mutex_t *forks;
	
	pthread_mutex_t *deadline_locks;
	pthread_mutex_t *eaten_locks;
	pthread_mutex_t *simdeath_lock;
	pthread_mutex_t *print_lock;
	pthread_mutex_t *race_gate;

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

	philo->deadline_lock = &(info->deadline_locks[i]);
	philo->eaten_lock = &(info->eaten_locks[i]);
	philo->simdeath_lock = info->simdeath_lock;
	philo->race_gate = info->race_gate;
	philo->print_lock = info->print_lock;
	
	// misc info
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
	*philosophers = malloc(sizeof(struct philosopher) * (info->num ));
	info->forks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->deadline_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->eaten_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->simdeath_lock = malloc(sizeof(pthread_mutex_t));
	info->race_gate = malloc(sizeof(pthread_mutex_t));
	info->print_lock = malloc(sizeof(pthread_mutex_t));

	if (!(*philosophers) || !info->forks || !info->deadline_locks || !info->eaten_locks || !info->simdeath_lock)
		return FUNC_FAIL;
	i = 0;
	while (i < info->num)
	{
		pthread_mutex_init(&(info->forks[i]), NULL);
		pthread_mutex_init(&(info->deadline_locks[i]), NULL);
		pthread_mutex_init(&(info->eaten_locks[i]), NULL);
		i++;
	}
	pthread_mutex_init(info->simdeath_lock, NULL);
	pthread_mutex_init(info->race_gate, NULL);
	pthread_mutex_init(info->print_lock, NULL);
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

int death_check(struct philosopher *philo)
{
	pthread_mutex_lock(philo->simdeath_lock);
	if (*(philo->sim_death))
	{
		pthread_mutex_unlock(philo->simdeath_lock);
		return 1;
	}
	pthread_mutex_unlock(philo->simdeath_lock);
	pthread_mutex_lock(philo->deadline_lock);
	if (philo->deadline < get_time(*philo->start))
	{
		pthread_mutex_unlock(philo->deadline_lock);
		return 1;
	}
	pthread_mutex_unlock(philo->deadline_lock);
	return 0;
}

int intermittent_sleep(struct philosopher *philo, uint64_t time)
{
	uint64_t wakeup;

	wakeup = get_time(*philo->start) + time;
	while (get_time(*philo->start) < wakeup)
	{
		usleep(500);
		pthread_mutex_lock(philo->deadline_lock);
		if (philo->deadline < get_time(*philo->start))
		{
			pthread_mutex_unlock(philo->deadline_lock);
			return 1;
		}
		pthread_mutex_unlock(philo->deadline_lock);
	}
	return 0;
}

int log_alive_action(struct philosopher *philo, char *str)
{
	pthread_mutex_lock(philo->print_lock);
	pthread_mutex_lock(philo->simdeath_lock);
	if (*philo->sim_death)
	{
		pthread_mutex_unlock(philo->simdeath_lock);
		pthread_mutex_unlock(philo->print_lock);
		return FUNC_FAIL;
	}	
	printf("%lu %d %s\n", get_time(*philo->start), philo->id, str);
	pthread_mutex_unlock(philo->simdeath_lock);
	pthread_mutex_unlock(philo->print_lock);
	return FUNC_SUCCESS;
}

void *philosophise(void *args)
{
	struct philosopher *philo;
	philo = (struct philosopher *)args;
	pthread_mutex_lock(philo->race_gate);
	pthread_mutex_unlock(philo->race_gate);
	// absolute death case -- 1 philosopher.
	if (philo->fork_left == philo->fork_right)
	{
		pthread_mutex_lock(philo->fork_left);
		log_alive_action(philo, "has taken a fork");
		while (1)
		{
			if (death_check(philo))
				break ;
			continue ;
		}
		pthread_mutex_unlock(philo->fork_left);
		return NULL;
	}
	usleep(philo->id % 2 * 50);
	while (1)
	{
		if (death_check(philo))
			break;
		if (philo->id % 2)
		{
			pthread_mutex_lock(philo->fork_left);
			philo->forks[0] = 1;
			if (!log_alive_action(philo, "has taken a fork"))
				break ;
			pthread_mutex_lock(philo->fork_right);
			philo->forks[1] = 1;
		}
		else
		{
			pthread_mutex_lock(philo->fork_right);
			philo->forks[1] = 1;
			if (death_check(philo))
				break;
			if (!log_alive_action(philo, "has taken a fork"))
				break ;
			pthread_mutex_lock(philo->fork_left);
			philo->forks[0] = 1;
		}
		if (!log_alive_action(philo, "has taken a fork"))
			break ;
		if (!log_alive_action(philo, "is eating"))
			break ;
		// extend deadline
		if (death_check(philo))
			break ;
		else
		{
			pthread_mutex_lock(philo->deadline_lock);
			philo->deadline = get_time(*philo->start) + philo->time_to_die;
			pthread_mutex_unlock(philo->deadline_lock);
		}
		if (intermittent_sleep(philo, philo->time_to_eat))
			break ;
		pthread_mutex_lock(philo->eaten_lock);
		philo->times_eaten += 1;
		pthread_mutex_unlock(philo->eaten_lock);

		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		philo->forks[0] = 0;
		philo->forks[1] = 0;
		// i sleep.
		if (!log_alive_action(philo, "is sleeping"))
			break ;
		if (intermittent_sleep(philo, philo->time_to_sleep))
			break ;
		// i think.
		if (!log_alive_action(philo, "is thinking"))
			break ;
		if ((philo->time_to_eat - philo->time_to_sleep) > 0)
		{
			if (intermittent_sleep(philo, philo->time_to_eat - philo->time_to_sleep))
				break ;
		}
	}
	if (philo->forks[0])
		pthread_mutex_unlock(philo->fork_left);
	if (philo->forks[1])
		pthread_mutex_unlock(philo->fork_right);
	return NULL;
}

int main(int argc, char **argv) 
{
	struct sim_info info;
	struct philosopher *philosophers;
	uint64_t start;

// temp

	info.death = 0;
	start = 0;
	// printf("argc: %d\n", argc);
	if (argc < 5 || argc > 6)
		return argc_fail();
	if (!init_setup(&info, &philosophers, argc, argv))
	{
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(info.race_gate);
	int i = 0;
	while (i < info.num)
	{
		philosophers[i].start = &start;
		pthread_create(&(philosophers[i].thread), NULL, philosophise, &(philosophers[i]));
		// pthread_detach(philosophers[i].thread);
		i++;
	}
	start = get_time(0);
	pthread_mutex_unlock(info.race_gate);
	// logic start
	while (1)
	{
		i = 0;
		while (i < info.num)
		{
			if (death_check(&(philosophers[i])))
			{
				pthread_mutex_lock(info.print_lock);
				pthread_mutex_lock(info.simdeath_lock);
				info.death = 1;
				printf("%lu %d died\n", get_time(start), philosophers[i].id);
				pthread_mutex_unlock(info.simdeath_lock);
				pthread_mutex_unlock(info.print_lock);

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
				pthread_mutex_lock(philosophers[i].eaten_lock);
				if (philosophers[i].times_eaten < info.minimum_eats)
				{
					pthread_mutex_unlock(philosophers[i].eaten_lock);
					break ;
				}
				if (i == info.num - 1)
				{
					pthread_mutex_lock(info.simdeath_lock);
					info.death = 1;
					pthread_mutex_unlock(info.simdeath_lock);
				}
				pthread_mutex_unlock(philosophers[i].eaten_lock);
				i++;
			}
			if (info.death)
				break;
		}
		usleep(100);
	}
	// logic end

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
