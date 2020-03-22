/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sleep.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 20:38:41 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 20:38:43 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "time.h"

int			sleep_mili(int miliseconds)
{
	struct timespec req = {0};

	req.tv_sec = 0;
	req.tv_nsec = miliseconds * 1000000L;

	return(nanosleep(&req, (struct timespec *)NULL));
}

int			sleep_micro(int microseconds)
{
	struct timespec req = {0};

	req.tv_sec = 0;
	req.tv_nsec = microseconds * 1000L;

	return(nanosleep(&req, (struct timespec *)NULL));
}

int			sleep_seconds(int seconds)
{
	struct timespec req = {0};

	req.tv_sec = seconds;
	req.tv_nsec = 0L;

	return(nanosleep(&req, (struct timespec *)NULL));
}
