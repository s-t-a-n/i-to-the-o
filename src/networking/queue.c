/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   queue.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 18:33:37 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/03 00:27:52 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

/*
** queue was inspired by (if not just ripped off from) https://github.com/Guppster/MultiThreaded-Server
*/

void		queue_drop(t_queue *queue)
{
	free(queue->elements);
	free(queue);
}

void		queue_pop(t_queue *queue)
{
	if (queue->size > 0)
	{
		if (queue->front < queue->cap)
		{
			(queue->front)++;
		}
		else
		{
			queue->front = 0;
		}
		(queue->size)--;
	}
}

void		queue_push(t_queue *queue, int element)
{
	if (queue->size < queue->cap)
	{
		if (queue->back < queue->cap)
		{
			(queue->back)++;
			(queue->size)++;
		}
		else
		{
			queue->back = 0;
			(queue->size)++;
		}
		queue->elements[queue->back] = element;
	}
}

int			queue_peek(t_queue *queue)
{
	return(queue->elements[queue->front]);
}

int			queue_count(t_queue *queue)
{
	return (queue->size);
}

void		queue_safe_add(t_queue *queue, int element)
{
	LOG_DEBUG("%s : %s\n", "queue_safe_add", "unlocking..");
	pthread_mutex_lock(&queue->lock);
	LOG_DEBUG("%s : %s\n", "queue_safe_add", "unlocked!");

	queue_push(queue, element);

	pthread_mutex_unlock(&queue->lock);

	pthread_cond_signal(&queue->signal);
}

int			queue_safe_get(t_queue *queue)
{
	int element;

	LOG_DEBUG("%s : %s\n", "queue_safe_get", "unlocking..");
	pthread_mutex_lock(&queue->lock);
	LOG_DEBUG("%s : %s\n", "queue_safe_get", "unlocked!");

	if (pthread_cond_wait(&queue->signal, &queue->lock) != 0)
		handle_error("queue_safe_get", "illegal signal:", strerror(errno),
				ERR_CRIT);

	element = queue_peek(queue);
	queue_pop(queue);
	pthread_mutex_unlock(&queue->lock);
	return (element);
}

t_queue		*queue_create(int cap)
{
	t_queue *queue;


	queue = malloc(sizeof(t_queue));
	if (queue)
	{
		queue->elements = malloc(sizeof(int) * cap);
		if (queue->elements)
		{
			queue->front = 0;
			queue->back = 0;
			queue->size = 0;
			queue->cap = cap;
			pthread_mutex_init(&queue->lock, NULL);
			pthread_cond_init(&queue->signal, NULL);
		}
		else
		{
			free(queue);
			queue = NULL;
		}
	}
	return(queue);
}
