/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:18:30 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 15:36:49 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int intermittent_sleep(t_philosopher *philo, uint64_t time)
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

int log_alive_action(t_philosopher *philo, char *str)
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

int death_check(t_philosopher *philo)
{
	pthread_mutex_lock(philo->simdeath_lock);
	if (*(philo->sim_death))
	{
		pthread_mutex_unlock(philo->simdeath_lock);
		return FUNC_SUCCESS;
	}
	pthread_mutex_unlock(philo->simdeath_lock);
	pthread_mutex_lock(philo->deadline_lock);
	if (philo->deadline < get_time(*philo->start))
	{
		pthread_mutex_unlock(philo->deadline_lock);
		return FUNC_SUCCESS;
	}
	pthread_mutex_unlock(philo->deadline_lock);
	return FUNC_FAIL;
}


