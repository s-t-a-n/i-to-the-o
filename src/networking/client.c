/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:25 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 16:05:23 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <stdlib.h>

#include "common/sleep.h"
#include "common/threading.h"
#include "networking/constants.h"
#include "networking/queue.h"
#include "networking/container.h"
#include "networking/client.h"

// NEEDED:
// f: client_request_access (no thread)
// interaction with pool

static int		process_request(t_container *container)
{
	ssize_t		bytes_written;
	ssize_t		bytes_written_actual;
	int			write_miss_counter;

	LOG_DEBUG("Client : Thread %d : %s\n", (int)pthread_self(), "processing request");
	bytes_written = 0;
	write_miss_counter = 0;
	while(bytes_written < container->vector->index)
	{
		bytes_written_actual = write(container->socketfd, container->vector->mem + bytes_written, container->vector->index);
		if (bytes_written_actual < 0)
		{
			handle_error("process_request", "couldn't send request!",
				strerror(errno), ERR_WARN);
			return(-1);
		}
		else if (bytes_written_actual == 0)
		{
			write_miss_counter++;
			if (write_miss_counter >= NT_WRITE_MAXRETRY)
			{
				handle_error("process_request", "Max times write attempt hit!",
						NULL, ERR_WARN);
				return (-1);
			}
			LOG_DEBUG("Client : Thread %d : %s\n", (int)pthread_self(), "bytes written was zero, sleeping..");
			sleep_micro(NT_WRITE_DELAY);
		}
		else
		{
			bytes_written += bytes_written_actual;
		}
	}
	return (1);
}

static void		*worker_requests(void *arg)
{
	t_client			*client = (t_client *)arg;
	t_container			*container;

	LOG_VERBOSE("Thread : %d, %s\n", (int)pthread_self(), "client is ready for requests!");
	while(client->state == NT_STATE_READY)
	{
		container = (t_container *)queue_safe_get(client->queue);
		process_request(container);
	}
	return (NULL);
}

void			client_shutdown(t_client *client)
{
	t_container	*container;

	LOG_DEBUG("%s\n", "shutting down client!");
	client->state = NT_STATE_STOP;
	pthread_mutex_lock(&client->queue->lock);
	while(client->queue->size > 0)
	{
		container = (t_container *)queue_peek(client->queue);
		mvector1_destroy(container->vector);
		free(container);
		queue_pop(client->queue);
	}
	queue_drop(client->queue);
	free(client->workers);
	free(client);
}

t_client		*client_initialise(void)
{
	t_client	*client;

	client = malloc(sizeof(t_client));
	if (client)
	{
		client->queue = queue_create(NT_QUEUE_CAP);
		if (!client->queue)
		{
			handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
			free(client);
			return(NULL);
		}

		client->state = NT_STATE_READY;

		client->workers = spin_up_threads(NT_WORKERS_DEF, NT_WORKERS_MAX, 0,
				worker_requests, (void *)client);
		if (!client->workers)
		{
			handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
			queue_drop(client->queue);
			free(client);
			return (NULL);
		}
	}
	else
		handle_error("initialise_client", strerror(errno), NULL, ERR_CRIT);
	return (client);
}
