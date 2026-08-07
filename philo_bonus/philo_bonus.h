/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:30:00 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/07 15:30:00 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdint.h>
# include <stdio.h> // printf
# include <stdlib.h> // malloc, free, exit
# include <string.h> // memset
# include <unistd.h> // write, fork, usleep
# include <sys/types.h> // pid_t
# include <sys/wait.h> // waitpid
# include <sys/time.h> // gettimeofday
# include <signal.h> // kill
# include <pthread.h> // pthread_create, pthread_detach
# include <semaphore.h> // sem_open, sem_close, sem_post, sem_wait, sem_unlink
# include <fcntl.h> // O_CREAT
# include <limits.h>

# define FUNC_SUCCESS 1
# define FUNC_FAIL 0

# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_SEATS "/philo_seats"

typedef _Atomic int			t_aint;
typedef _Atomic uint64_t	t_au64;

typedef struct s_sim
{
	int			num;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			minimum_eats;
	uint64_t	start;
	pid_t		*pids;
	sem_t		*forks;
	sem_t		*print;
	sem_t		*seats;
}	t_sim;

typedef struct s_philo
{
	int				id;
	t_au64			deadline;
	t_aint			times_eaten;
	struct s_sim	*info;
}	t_philo;

int			ft_isdigit(int c);
int			ft_atol(const char *nptr);
int			validate_args(struct s_sim *info, int argc, char **argv);
int			init_info(struct s_sim *info, int argc, char **argv);

uint64_t	get_time(uint64_t start);
void		ft_usleep(uint64_t start, uint64_t time);

void		log_action(struct s_philo *philo, char *str);
void		log_death(struct s_philo *philo);

void		child_process(struct s_sim *info, int id);
void		child_exit(struct s_sim *info, int status);
void		*monitor_thread(void *arg);
void		kill_all(struct s_sim *info);
void		reaper(struct s_sim *info);

#endif
