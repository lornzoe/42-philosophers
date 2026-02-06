/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:21:20 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 15:46:18 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void init_a_philo(t_philosopher *philo, t_sim *info, int i)
{
	philo->times_eaten = 0;

	philo->time_to_die = info->time_to_die;
	philo->time_to_eat = info->time_to_eat;
	philo->time_to_sleep = info->time_to_sleep;
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
	philo->id = i + 1;
	philo->deadline = info->time_to_die;
	philo->sim_death = &(info->death);
	philo->forks[0] = 0;
	philo->forks[1] = 0;
}

static void init_info(t_sim *info, int argc, char **argv)
{
	info->num = atoi(argv[1]);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		info->minimum_eats = atoi(argv[5]);
	else
		info->minimum_eats = -1;
	info->forks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->deadline_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->eaten_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->simdeath_lock = malloc(sizeof(pthread_mutex_t));
	info->race_gate = malloc(sizeof(pthread_mutex_t));
	info->print_lock = malloc(sizeof(pthread_mutex_t));
}

int init_setup(t_sim *info, t_philosopher **philosophers, int argc, char **argv)
{
	int i;

	init_info(info, argc, argv);
	*philosophers = malloc(sizeof(t_philosopher) * (info->num ));
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
	return FUNC_SUCCESS;
}
