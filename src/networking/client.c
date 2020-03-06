/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:25 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/06 20:45:42 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

// NEEDED:
// f: client_request_access (no thread)
// interaction with pool

static int		process_request(t_package_nt *package)
{
	ssize_t		bytes_written;
	ssize_t		bytes_written_actual;
	int			write_miss_counter;

	LOG_DEBUG("Client : Thread %d : %s\n", (int)pthread_self(), "processing request");
	bytes_written = 0;
	write_miss_counter = 0;
	while(bytes_written < package->index)
	{
		bytes_written_actual = write(package->socketfd, package->mem + bytes_written, package->index);
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
			LOG_DEBUG("Client : Thread %d : %s\n", (int)pthread_self(), "bytes written was zero, sleeping.");
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
	t_package_nt		*package;

	LOG_VERBOSE("Thread : %d, %s\n", (int)pthread_self(), "client is ready for requests!");
	while(client->state == NT_STATE_READY)
	{
		package = (t_package_nt *)queue_safe_get(client->queue);
		process_request(package);
	}
	return (NULL);
}

void			shutdown_client(t_client *client)
{
	t_package_nt	*package;

	LOG_DEBUG("%s\n", "shutting down client!");
	client->state = NT_STATE_STOP;
	pthread_mutex_lock(&client->queue->lock);
	while(client->queue->size > 0)
	{
		package = queue_peek(client->queue);
		free(package->mem);
		free(package);
		queue_pop(client->queue);
	}
	queue_drop(client->queue);
	free(client->thread_tab);
	free(client);
}

t_client		*initialise_client(void)
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
		client->thread_tab = malloc(sizeof(pthread_t) * NT_WORKERS_MAX);
		if (client->thread_tab)
		{
			client->state = NT_STATE_READY;
			// redo SPIN UP with threads.h
			/*
			if (!spin_up_threads(client->thread_tab, client))
			{
				handle_error("initialise_client", "couldn't spin up threads!",
						strerror(errno), ERR_CRIT);
				queue_drop(client->queue);
				free(client->thread_tab);
				free(client);
				return(NULL);
			}
			*/
		}
		else
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
