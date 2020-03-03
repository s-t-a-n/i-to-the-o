/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:31 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 00:42:43 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

static int		process_request(int descriptor, t_server *server)
{
	char		recv_buffer[NT_BUF_SIZE + 1];
	int			bytes_received;

	LOG_DEBUG("Thread %d : %s : %i\n", (int)pthread_self(), "processing request", descriptor);
	bytes_received = recv(descriptor, recv_buffer, NT_BUF_SIZE, 0);
	if (bytes_received < 0)
		handle_error("process_request", "problem receiving message",
				strerror(errno), ERR_WARN);
	else
	{
		recv_buffer[bytes_received] = '\0';
		// do all processing here
		server = NULL;
		if (close(descriptor) < 0)
			handle_error("process_request", "couldnt close socket:",
					strerror(errno), ERR_WARN);
	}
	return (0);
}

static void		*worker_requests(void *arg)
{
	t_server	*server = (t_server *)arg;
	int			*descriptor;

	while (server->state == NT_STATE_READY)
	{
		descriptor = (int *)queue_safe_get(server->queue);
		if (descriptor)
		{
			process_request(*descriptor, server);
			free(descriptor);
		}
	}

	return (arg);
}

static void		*worker_incoming(void *arg)
{
	t_server	*server = (t_server *)arg;
	int			*descriptor;

	if (listen(server->socket, NT_QUEUE_BACKLOG) != 0)
	{
		server->state = NT_STATE_STOP;
		handle_error("worker_incoming", "couldnt listen on socket!",
				strerror(errno), ERR_CRIT);
	}
	else
	{
		LOG_VERBOSE("Thread : %d, %s\n", (int)pthread_self(), "server is accepting requests!");
		while (server->state == NT_STATE_READY)
		{
			descriptor = malloc(sizeof(int));
			if (descriptor)
			{
				*descriptor = accept(server->socket, (struct sockaddr *)NULL, NULL);
				if (*descriptor < 0 && errno != ECONNABORTED)
					handle_error("worker_incoming", "problem with incoming request",
							strerror(errno), ERR_WARN);
				else
				{
					queue_safe_add(server->queue, (void *)descriptor);
				}
			}
			else
				handle_error("worker_requests", strerror(errno), NULL, ERR_CRIT);
		}
	}
	return(NULL);
}

static int		spin_up_threads(pthread_t *thread_tab, t_server *server)
{
	int ctr = 1;

	LOG_DEBUG("%s : %s\n", "server", "spinning up threads!");
	if (pthread_create(&thread_tab[0], NULL, worker_incoming,
				(void *)server) != 0)
		return(0);
	while (ctr < NT_WORKERS_DEF + 1)
	{
		if (pthread_create(&thread_tab[ctr], NULL, worker_requests,
					(void *)server) != 0)
		{
			int undo_ctr = 0;
			while (undo_ctr < ctr)
			{
				pthread_kill(thread_tab[undo_ctr], SIGTERM);
				undo_ctr++;
			}
			return (0);
		}
		ctr++;
	}
	return (1);
}

void			shutdown_server(t_server *server)
{
	LOG_DEBUG("%s\n", "shutting down server!");
	if (close(server->socket) < 0)
		handle_error("shutdown_server", "couldnt close socket!",
				strerror(errno), ERR_CRIT);
	while(server->queue->size > 0)
	{
		if (close(*(int *)queue_peek(server->queue)) < 0)
			handle_error("shutdown_server", "couldnt close socket!",
					strerror(errno), ERR_CRIT);
		queue_pop(server->queue);
	}
	queue_drop(server->queue);
	free(server->thread_tab);
	free(server);
}

t_server		*initialise_server(void)
{
	t_server *server;

	LOG_DEBUG("%s\n", "starting server!");
	server = malloc(sizeof(t_server));
	if (server)
	{
		server->queue = queue_create(NT_QUEUE_CAP);
		if (!server->queue)
		{
			handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
			free(server);
			return(NULL);
		}

		server->thread_tab = malloc(sizeof(pthread_t) * (NT_WORKERS_MAX + 1));
		if (server->thread_tab)
		{
			server->socket = socket(AF_INET, SOCK_STREAM, 0);
			if (server->socket < 0)
			{
				handle_error("initialise_server", "couldnt open socket!",
						strerror(errno), ERR_CRIT);
				free(server->thread_tab);
				queue_drop(server->queue);
				free(server);
				return(NULL);
			}

			if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
			{
				handle_error("initialise_server", "couldnt set socket options!",
						strerror(errno), ERR_CRIT);
				queue_drop(server->queue);
				close(server->socket);
				free(server->thread_tab);
				free(server);
				return(NULL);
			}

			server->address.sin_family = AF_INET;
			server->address.sin_port = htons(NT_PORT);
			server->address.sin_addr.s_addr = htonl(INADDR_ANY);
			memset(&server->address.sin_zero, 0, sizeof(server->address.sin_zero));

			if (bind(server->socket, (struct sockaddr *)&server->address,
						sizeof(server->address)) < 0)
			{
				handle_error("initialise_server", "couldn't bind to socket!",
						strerror(errno), ERR_CRIT);
				close(server->socket);
				queue_drop(server->queue);
				free(server->thread_tab);
				free(server);
				return(NULL);
			}

			server->state = NT_STATE_READY;
			if (!spin_up_threads(server->thread_tab, server))
			{
				handle_error("initialise_server", "couldn't spin up threads!",
						strerror(errno), ERR_CRIT);
				close(server->socket);
				queue_drop(server->queue);
				free(server->thread_tab);
				free(server);
				return (NULL);
			}
		}
		else
		{
			handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
			queue_drop(server->queue);
			free(server);
			return(NULL);
		}
	}
	else
		handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
	return (server);
}
