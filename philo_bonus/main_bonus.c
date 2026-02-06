/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:45:57 by lyanga            #+#    #+#             */
/*   Updated: 2026/01/15 15:48:02 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>    // memset
#include <stdio.h>     // printf
#include <stdlib.h>    // malloc, free, exit
#include <unistd.h>    // write, fork, usleep
#include <sys/types.h> // pid_t (used by fork, waitpid)
#include <sys/wait.h>  // waitpid
#include <signal.h>    // kill
#include <sys/time.h>  // gettimeofday
#include <pthread.h>   // pthread_create, pthread_detach, pthread_join
#include <semaphore.h> // sem_open, sem_close, sem_post, sem_wait, sem_unlink
#include <fcntl.h>     // O_CREAT (used with sem_open)

struct philosopher 
{
	int id;
	
	u_int64_t deadline;
	int times_eaten;
	
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int forks_held;

	pthread_mutex_t *deadline_lock;
	pthread_mutex_t *eaten_lock;
	pthread_mutex_t *simdeath_lock;
	pthread_mutex_t *print_lock;
	pthread_mutex_t *race_gate;
	
	u_int64_t *start;

	int *sim_death;
	// int thread_end;
};

struct sim_info
{
	int num;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int minimum_eats;
	pthread_mutex_t *forks;
	
	pthread_mutex_t *deadline_locks;
	pthread_mutex_t *eaten_locks;
	pthread_mutex_t *simdeath_lock;
	pthread_mutex_t *print_lock;
	pthread_mutex_t *race_gate;

	int death;
};

static uint64_t get_time(uint64_t start)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t)tv.tv_sec * 1000 + (tv.tv_usec / 1000) - start);
}


void philosophise(void)
{

}

static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}

int main(int argc, char **argv) 
{
	if (argc < 5 || argc > 6)
	return argc_fail();


	// init 

	// release the forks
	
	// go go go
	return (0);
}
