/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:43:30 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 20:06:25 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdint.h>

static int	pick_up_forks(t_philosopher *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->fork_left);
		philo->forks[0] = 1;
		if (!log_alive_action(philo, "has taken a fork"))
			return (FUNC_FAIL);
		pthread_mutex_lock(philo->fork_right);
		philo->forks[1] = 1;
	}
	else
	{
		pthread_mutex_lock(philo->fork_right);
		philo->forks[1] = 1;
		if (death_check(philo))
			return (FUNC_FAIL);
		if (!log_alive_action(philo, "has taken a fork"))
			return (FUNC_FAIL);
		pthread_mutex_lock(philo->fork_left);
		philo->forks[0] = 1;
	}
	return (FUNC_SUCCESS);
}

int	philo_eat(t_philosopher *philo)
{
	if (!pick_up_forks(philo))
		return (FUNC_FAIL);
	if (!log_alive_action(philo, "has taken a fork"))
		return (FUNC_FAIL);
	if (!log_alive_action(philo, "is eating"))
		return (FUNC_FAIL);
	if (death_check(philo))
		return (FUNC_FAIL);
	else
	{
		pthread_mutex_lock(philo->deadline_lock);
		philo->deadline = get_time(*philo->start) + philo->time_to_die;
		pthread_mutex_unlock(philo->deadline_lock);
	}
	if (!intermittent_sleep(philo, philo->time_to_eat))
		return (FUNC_FAIL);
	pthread_mutex_lock(philo->eaten_lock);
	philo->times_eaten += 1;
	pthread_mutex_unlock(philo->eaten_lock);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
	philo->forks[0] = 0;
	philo->forks[1] = 0;
	return (FUNC_SUCCESS);
}

int	philo_sleep(t_philosopher *philo)
{
	if (!log_alive_action(philo, "is sleeping"))
		return (FUNC_FAIL);
	if (!intermittent_sleep(philo, philo->time_to_sleep))
		return (FUNC_FAIL);
	return (FUNC_SUCCESS);
}

int	philo_think(t_philosopher *philo)
{
	uint64_t	diff;

	if (!log_alive_action(philo, "is thinking"))
		return (FUNC_FAIL);
	if ((philo->time_to_eat - philo->time_to_sleep) > 0)
	{
		if (!intermittent_sleep(philo,
				philo->time_to_eat - philo->time_to_sleep))
			return (FUNC_FAIL);
	}
	if (get_time(*(philo->start) < philo->deadline))
	{
		diff = philo->deadline - get_time(*(philo->start));
		diff /= 10;
		if (diff > 0)
		{
			if (!intermittent_sleep(philo, diff))
				return (FUNC_FAIL);
		}
	}
	return (FUNC_SUCCESS);
}
