/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:21:20 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 17:46:17 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (*s == '\0')
		return (0);
	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}

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

static int validate_args(t_sim *info, int argc, char **argv)
{
	int i;

	i = 1;
	while (i < argc)
	{
		if (ft_strlen(argv[i]) > 10)
			return FUNC_FAIL;
		while (*(argv[i]) != '\0')
		{
			if (!ft_isdigit(*(argv[i])))
				return FUNC_FAIL;
			(argv[i])++;
		}
		i++;
	}
	if (info->num <= 0 || info->time_to_die <= 0 || info->time_to_eat <= 0 || info->time_to_sleep <= 0)
		return FUNC_FAIL;
	if (argc == 6 && info->minimum_eats <= 0)
		return FUNC_FAIL;
	return FUNC_SUCCESS;
}

static int init_info(t_sim *info, int argc, char **argv)
{
	info->forks = NULL;
	info->deadline_locks = NULL;
	info->eaten_locks = NULL;
	info->simdeath_lock = NULL;
	info->print_lock = NULL;
	info->race_gate = NULL;
	info->num = ft_atol(argv[1]);
	info->time_to_die = ft_atol(argv[2]);
	info->time_to_eat = ft_atol(argv[3]);
	info->time_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		info->minimum_eats = ft_atol(argv[5]);
	else
		info->minimum_eats = -1;
	if (!validate_args(info, argc, argv))
	{
		printf("failed here");
		return FUNC_FAIL;
	}
		
	info->forks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->deadline_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->eaten_locks = malloc(sizeof(pthread_mutex_t) * (info->num));
	info->simdeath_lock = malloc(sizeof(pthread_mutex_t));
	info->print_lock = malloc(sizeof(pthread_mutex_t));
	info->race_gate = malloc(sizeof(pthread_mutex_t));
	if (!info->forks || !info->deadline_locks || !info->eaten_locks || !info->simdeath_lock || !info->print_lock || !info->race_gate)
		return FUNC_FAIL;
	return FUNC_SUCCESS;
}

int init_setup(t_sim *info, t_philosopher **philosophers, int argc, char **argv)
{
	int i;
	if (!init_info(info, argc, argv))
		return FUNC_FAIL;
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
