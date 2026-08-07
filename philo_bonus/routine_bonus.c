/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 20:18:08 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	child_exit(t_sim *info, int status)
{
	(void)info;
	exit(status);
}

static void	take_forks(t_philo *philo)
{
	sem_wait(philo->info->seats);
	sem_wait(philo->info->forks);
	log_action(philo, "has taken a fork");
	sem_wait(philo->info->forks);
	log_action(philo, "has taken a fork");
}

static void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	log_action(philo, "is eating");
	philo->deadline = get_time(philo->info->start) + philo->info->time_to_die;
	ft_usleep(philo->info->start, philo->info->time_to_eat);
	philo->times_eaten += 1;
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
	sem_post(philo->info->seats);
}

static void	philo_sleep_think(t_philo *philo)
{
	uint64_t	now;
	uint64_t	diff;

	log_action(philo, "is sleeping");
	ft_usleep(philo->info->start, philo->info->time_to_sleep);
	log_action(philo, "is thinking");
	if (philo->info->time_to_eat > philo->info->time_to_sleep)
		ft_usleep(philo->info->start,
			philo->info->time_to_eat - philo->info->time_to_sleep);
	now = get_time(philo->info->start);
	if (now < philo->deadline)
	{
		diff = (philo->deadline - now) / 10;
		if (diff > 0)
			ft_usleep(philo->info->start, diff);
	}
}

void	child_process(t_sim *info, int id)
{
	t_philo		philo;
	pthread_t	watcher;

	free(info->pids);
	info->pids = NULL;
	philo.id = id;
	philo.info = info;
	philo.times_eaten = 0;
	philo.deadline = info->time_to_die;
	if (pthread_create(&watcher, NULL, monitor_thread, &philo))
		child_exit(info, EXIT_FAILURE);
	pthread_detach(watcher);
	if (id % 2 == 0)
		usleep(300);
	while (1)
	{
		philo_eat(&philo);
		if (info->minimum_eats > 0 && philo.times_eaten >= info->minimum_eats)
			break ;
		philo_sleep_think(&philo);
	}
	child_exit(info, EXIT_SUCCESS);
}
