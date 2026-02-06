/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 15:38:40 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 15:38:48 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int monitor_death(t_sim *info, t_philosopher *philosophers, uint64_t start)
{
	int i;

	i = 0;
	while (i < info->num)
	{
		if (death_check(&(philosophers[i])))
		{
			pthread_mutex_lock(info->print_lock);
			pthread_mutex_lock(info->simdeath_lock);
			info->death = 1;
			printf("%lu %d died\n", get_time(start), philosophers[i].id);
			pthread_mutex_unlock(info->simdeath_lock);
			pthread_mutex_unlock(info->print_lock);
			return FUNC_SUCCESS;
		}
		i++;
	}
	return FUNC_FAIL;
}

static int monitor_eats(t_sim *info, t_philosopher *philosophers)
{
    int i;

    if (info->minimum_eats == -1)
        return (FUNC_FAIL);
    i = 0;
    while (i < info->num)
    {
        pthread_mutex_lock(philosophers[i].eaten_lock);
        if (philosophers[i].times_eaten < info->minimum_eats)
        {
            pthread_mutex_unlock(philosophers[i].eaten_lock);
            return (FUNC_FAIL);
        }
        pthread_mutex_unlock(philosophers[i].eaten_lock);
        i++;
    }
    pthread_mutex_lock(info->simdeath_lock);
    info->death = 1;
    pthread_mutex_unlock(info->simdeath_lock);
    return (FUNC_SUCCESS);
}

void monitoring(t_sim *info, t_philosopher *philosophers, uint64_t start)
{
	while (1)
	{
		if (monitor_death(info, philosophers, start))
			break ;
		if (info->death)
			break ;
		if (info->minimum_eats > -1)
		{
			if (monitor_eats(info, philosophers))
				break ;
			if (info->death)
			break;
		}
		usleep(100);
	}
}
