/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 19:43:54 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 17:20:52 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADING_H
# define THREADING_H

# include <pthread.h>
# include <signal.h>
# include "ito_internal.h"

void				spin_down_threads(int amount, pthread_t *threads);
pthread_t			spin_up_thread(pthread_attr_t *attr,
						void *(*main)(void *), void *arg);
pthread_t			*spin_up_threads(int amount, int maxamount,
						pthread_attr_t *attr, void *(*main)(void *),
						void *arg);

#endif
