/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:18:30 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 19:31:33 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_time(uint64_t start)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000) - start);
}

int	intermittent_sleep(t_philosopher *philo, uint64_t time)
{
	uint64_t	wakeup;

	wakeup = get_time(philo->start) + time;
	while (get_time(philo->start) < wakeup)
	{
		usleep(500);
		if (philo->deadline < get_time(philo->start))
			return (FUNC_FAIL);
	}
	return (FUNC_SUCCESS);
}

int	log_alive_action(t_philosopher *philo, char *str)
{
	pthread_mutex_lock(philo->print_lock);
	if (*(philo->sim_death))
	{
		pthread_mutex_unlock(philo->print_lock);
		return (FUNC_FAIL);
	}
	printf("%lu %d %s\n", get_time(philo->start), philo->id, str);
	pthread_mutex_unlock(philo->print_lock);
	return (FUNC_SUCCESS);
}

int	death_check(t_philosopher *philo)
{
	if (*(philo->sim_death))
		return (FUNC_SUCCESS);
	if (philo->deadline < get_time(philo->start))
		return (FUNC_SUCCESS);
	return (FUNC_FAIL);
}
