/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 12:14:57 by lyanga            #+#    #+#             */
/*   Updated: 2026/02/06 20:25:21 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <inttypes.h>
# include <stdint.h>
# include <stdio.h> // printf
# include <stdlib.h> // malloc, free
# include <sys/types.h>
# include <unistd.h> // write, usleep
# include <sys/time.h> // gettimeofday
# include <pthread.h> // pthread funcs (3) + pthread_mutex funcs (4)
# include <limits.h>

# define FUNC_SUCCESS 1
# define FUNC_FAIL 0

typedef struct s_philosopher
{
	pthread_t		thread;
	int				id;
	u_int64_t		deadline;
	int				times_eaten;	
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				forks[2];
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	*deadline_lock;
	pthread_mutex_t	*eaten_lock;
	pthread_mutex_t	*simdeath_lock;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*race_gate;
	u_int64_t		*start;
	int				*sim_death;
}	t_philosopher;

typedef struct s_sim
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				minimum_eats;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*deadline_locks;
	pthread_mutex_t	*eaten_locks;
	pthread_mutex_t	*simdeath_lock;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*race_gate;
	int				death;
}	t_sim;

int			ft_isdigit(int c);
int			ft_atol(const char *nptr);
int			validate_args(t_sim *info, int argc, char **argv);
int			init_setup(struct s_sim *info, struct s_philosopher **philosophers,
				int argc, char **argv);

void		monitoring(struct s_sim *info, struct s_philosopher *philosophers,
				uint64_t start);

void		*philosophise(void *args);
int			philo_eat(struct s_philosopher *philo);
int			philo_sleep(struct s_philosopher *philo);
int			philo_think(struct s_philosopher *philo);

uint64_t	get_time(uint64_t start);
int			death_check(struct s_philosopher *philo);
int			log_alive_action(struct s_philosopher *philo, char *str);
int			intermittent_sleep(struct s_philosopher *philo, uint64_t time);

#endif