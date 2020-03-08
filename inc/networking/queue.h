/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   queue.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:08:16 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:19:04 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

# include <stdlib.h>
# include <pthread.h>
# include <signal.h>

# include "ito_internal.h"

typedef struct				s_queue
{
	pthread_mutex_t			lock;
	pthread_cond_t			signal;
	int						cap;
	int						size;
	int						front;
	int						back;
	void					**elements;
}							t_queue;

void						queue_drop(t_queue *queue);
void						queue_pop(t_queue *queue);
void						queue_push(t_queue *queue, void *element);
void						*queue_peek(t_queue *queue);
void						*queue_safe_get(t_queue *queue);
void						queue_safe_add(t_queue *queue, void *element);
t_queue						*queue_create(int cap);

#endif
