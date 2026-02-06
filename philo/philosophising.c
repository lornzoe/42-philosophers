/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophising.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:27:45 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 19:03:20 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_alone(t_philosopher *philo)
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
	return (NULL);
}

void	*philosophise(void *args)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)args;
	pthread_mutex_lock(philo->race_gate);
	pthread_mutex_unlock(philo->race_gate);
	if (philo->fork_left == philo->fork_right)
		return (philo_alone(philo));
	if (!(philo->id % 2))
		usleep(300);
	while (1)
	{
		if (death_check(philo))
			break ;
		if (!philo_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		if (!philo_think(philo))
			break ;
	}
	if (philo->forks[0])
		pthread_mutex_unlock(philo->fork_left);
	if (philo->forks[1])
		pthread_mutex_unlock(philo->fork_right);
	return (NULL);
}
