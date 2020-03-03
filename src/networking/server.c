/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:31 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/03 00:23:51 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

static int		process_request(int descriptor, t_server *server)
{
	LOG_DEBUG("Thread %lu : %s\n", pthread_self(), "processing request!");
	server = NULL;
	descriptor = 2;
	return (0);
}

static void		*worker_requests(void *arg)
{
	t_server	*server = (t_server *)arg;
	int			descriptor;

	while (server->state == NT_STATE_READY)
	{
		descriptor = queue_safe_get(server->queue);
		process_request(descriptor, server);
	}
	return (arg);
}

static void		*worker_incoming(void *arg)
{
	t_server	*server = (t_server *)arg;
	int			descriptor;

	if (listen(server->socket, NT_QUEUE_BACKLOG) != 0)
	{
		server->state = NT_STATE_STOP;
		handle_error("worker_incoming", "couldnt listen on socket!",
				strerror(errno), ERR_CRIT);
	}
	else
	{
		LOG_VERBOSE("Thread : %lu, %s\n", pthread_self(), "server is accepting requests!");
		while (server->state == NT_STATE_READY)
		{
			descriptor = accept(server->socket, (struct sockaddr *)NULL, NULL);
			if (descriptor < 0 && errno != ECONNABORTED)
				handle_error("worker_incoming", "problem with incoming request",
					strerror(errno), ERR_WARN);
			else
			{
				LOG_DEBUG("%s\n", "adding incoming request to queue!");
				queue_safe_add(server->queue, descriptor);
			}
		}
	}
	return(NULL);
}

static int		spin_up_threads(pthread_t *thread_tab, t_server *server)
{
	int ctr = 1;

	LOG_DEBUG("%s\n", "spinning up threads!");

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
				free(server->thread_tab);
				free(server);
				return(NULL);
			}

			server->state = NT_STATE_READY;
			if (!spin_up_threads(server->thread_tab, server))
				handle_error("initialise_server", "couldn't spin up threads!",
						strerror(errno), ERR_CRIT);
		}
		else
		{
			handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
			free(server);
			return(NULL);
		}
	}
	return (server);
}
