/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   queue.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 18:33:37 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 00:29:22 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

/*
** queue was inspired by (if not just ripped off from) https://github.com/Guppster/MultiThreaded-Server
*/

void		queue_drop(t_queue *queue)
{
	while (queue->size > 0)
	{
		free(queue_peek(queue));
		queue_pop(queue);
	}
	free(queue->elements);
	free(queue);
}

void		queue_pop(t_queue *queue)
{
	queue->elements[queue->front] = NULL;
	if (queue->size > 0)
	{
		if (queue->front < queue->cap - 1)
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

void		queue_push(t_queue *queue, void *element)
{
	if (queue->size < queue->cap)
	{
		if (queue->back < queue->cap - 1)
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
	else
		handle_error("queue_push", "queue is overflowing!", NULL, ERR_WARN);

	LOG_DEBUG("%s : %s : %i\n", "queue_push", "pushed element, size is now:", queue->size);
}

void		*queue_peek(t_queue *queue)
{
	return(queue->elements[queue->front]);
}

int			queue_count(t_queue *queue)
{
	return (queue->size);
}

void		queue_safe_add(t_queue *queue, void *element)
{
	pthread_mutex_lock(&queue->lock);
	queue_push(queue, element);
	pthread_mutex_unlock(&queue->lock);
	pthread_cond_signal(&queue->signal);
}

void		*queue_safe_get(t_queue *queue)
{
	void *element;

	pthread_mutex_lock(&queue->lock);
	while (queue->size == 0)
	{
		if (pthread_cond_wait(&queue->signal, &queue->lock) != 0)
			handle_error("queue_safe_get", "illegal signal:", strerror(errno),
					ERR_CRIT);
	}
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
		queue->elements = calloc(sizeof(void *), cap);
		if (queue->elements)
		{
			queue->front = 0;
			queue->back = -1;
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
