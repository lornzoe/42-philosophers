/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 21:45:57 by lyanga            #+#    #+#             */
/*   Updated: 2026/01/09 03:06:59 by lyanga           ###   ########.fr       */
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


static int argc_fail(void)
{
	printf("usage: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
	return EXIT_FAILURE;
}

int main(int argc, char **argv) 
{
	if (argc < 5 || argc > 6)
	return argc_fail();

	return (0);
}
