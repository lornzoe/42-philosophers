/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_min.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 09:57:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/01/03 09:57:07 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memset
#include <sys/types.h>
#include <unistd.h> // write, usleep
#include <sys/time.h> // gettimeofday
#include <pthread.h> // pthread funcs (3) + pthread_mutex funcs (4)

// bufferUS is a constant buffer time in microseconds that is meant to give the program leeway time to act
#define BUFFERUS 100

void sleep_for(uint64_t sleepeatdiff, uint64_t timeleft, uint64_t percentage, uint64_t ttd)
{
    // if time to sleep is shorter than time to eat, minimum time to usleep should be the diff + BUFFERUS
    if (sleepeatdiff > 0)
        usleep(sleepeatdiff * 1000);
    if ((timeleft / 100 * percentage < ttd))
        usleep(ttd * 1000 - BUFFERUS)
    else
        usleep(timeleft * 10 * percentage - BUFFERUS);
}