/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threading.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 19:43:54 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 17:20:52 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADING_H
# define THREADING_H

void				spin_down_threads(int amount,
									pthread_t *threads);
pthread_t			spin_up_thread(const pthread_attr_t *attr,
									void *(*main)(void *),
									void *arg);
pthread_t			*spin_up_threads(int amount,
									int cap,
									const pthread_attr_t *attr,
									void *(*main)(void *),
									void *arg);

#endif
