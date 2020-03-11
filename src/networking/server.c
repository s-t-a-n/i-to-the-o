/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 20:21:31 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:26:02 by sverschu      ########   odam.nl         */
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
#include <string.h>

#include "ito_internal.h"
#include "common/sleep.h"
#include "common/threading.h"
#include "networking/network.h"
#include "networking/queue.h"
#include "networking/constants.h"
#include "networking/pooling.h"
#include "networking/framing.h"
#include "networking/requests.h"
#include "networking/node.h"

static unsigned char	*recv_frame_header(int descriptor, unsigned char *frame_buffer)
{
	ssize_t					bytes_read;
	ssize_t					bytes_read_actual;
	int						read_miss_counter;

	bytes_read = 0;
	read_miss_counter = 0;
	while(bytes_read < FRAME_HEADER_LEN)
	{
		bytes_read_actual = recv(descriptor, frame_buffer + bytes_read,
				FRAME_HEADER_LEN - bytes_read, 0);
		if (bytes_read_actual < 0)
		{
			handle_error("Sever: process_request", "couldn't read request!",
					strerror(errno), ERR_WARN);
			return (NULL);
		}
		else if (bytes_read_actual == 0)
		{
			read_miss_counter++;
			if (read_miss_counter >= NT_READ_MAXRETRY)
			{
				handle_error("Server: process_request",
						"Frame header couldn't be read!", NULL, ERR_WARN);
				return (NULL);
			}
			LOG_DEBUG("Server : Thread %d : %s\n", (int)pthread_self(),
					"bytes read was zero, sleeping..");
			sleep_micro(NT_READ_DELAY);
		}
		else
			bytes_read += bytes_read_actual;
	}
	return (frame_buffer);
}

static int		process_request(t_node *node, t_network *network)
{
	unsigned char			frame_buffer[NT_RECV_BUFSIZE];
	t_request_type			reqtype;

	LOG_DEBUG("Server : Thread %d : %s : %i\n", (int)pthread_self(),
			"processing request", node->socketfd);
	if (!recv_frame_header(node->socketfd, frame_buffer))
	{
		handle_error("Server : process_request", "couldn't read frame heaeder!",
				NULL,  ERR_WARN);
		close(node->socketfd);
		return (-1);
	}
	if (frame_validate_signature(frame_buffer) <= 0)
	{
		handle_error("Server : process_request", "frame has invalid signature!",
				NULL,  ERR_WARN);
		close(node->socketfd);
		return (-1);
	}

	reqtype = frame_read_reqtype(frame_buffer);

	switch (reqtype)
	{
		case JOIN:
			return(process_join_rq(node, frame_buffer, network));
		case PACKAGE:
			return(process_package_rq(node, frame_buffer, network));
		case ILLEGAL:
			handle_error("Server : process request", "frame has illegal request type!", NULL,  ERR_WARN);
			return (-1);
		default:
			handle_error("Server : process request", "frame has invalid request type!", NULL,  ERR_WARN);
			return (-1);
	}
	return (1);
}

static void		*worker_requests(void *arg)
{
	t_network	*network = (t_network *)arg;
	t_server	*server = network->server;
	t_node	*node;

	while (server->state == NT_STATE_READY)
	{
		node = (t_node *)queue_safe_get(server->queue);
		if (node)
		{
			process_request(node, network);
			// BIG FAT MEMORY LEAK HERE -> WHEN TO FREE?
			free(node);
		}
	}
	return (arg);
}

// cleanup this function
// store socketfd -> send it to pool
static void		*worker_incoming(void *arg)
{
	t_network	*network = (t_network *)arg;
	t_server	*server = network->server;
	t_node *node;

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
			node = malloc(sizeof(t_node));
			if (node)
			{
				node->socklen = sizeof(struct sockaddr_in);
				node->socketfd = accept(server->socket, (struct sockaddr *)&node->sockaddr_in, &node->socklen);
				if (node->socketfd < 0 && errno != ECONNABORTED)
				{
					handle_error("worker_incoming", "problem with incoming request",
							strerror(errno), ERR_WARN);
					free(node);
				}
				else
				{
					int error = 0;
					error += setsockopt(node->socketfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
					error += setsockopt(node->socketfd, SOL_SOCKET, SO_KEEPALIVE, &(int){1}, sizeof(int));
					if (error > 0)
					{
						handle_error("worker_incoming", "couldnt set socket opts:", strerror(errno), ERR_WARN);
						close(node->socketfd);
						free(node);
					}
					else
						queue_safe_add(server->queue, (void *)node);
				}
			}
			else
				handle_error("worker_requests", strerror(errno), NULL, ERR_CRIT);
		}
	}
	return(NULL);
}

void			server_shutdown(t_server *server)
{
	LOG_DEBUG("%s\n", "shutting down server!");
	server->state = NT_STATE_STOP;
	pthread_mutex_lock(&server->queue->lock);
	if (close(server->socket) < 0)
		handle_error("shutdown_server", "couldnt close socket properly!",
				strerror(errno), ERR_CRIT);
	while(server->queue->size > 0)
	{
		if (close(*(int *)queue_peek(server->queue)) < 0)
			handle_error("shutdown_server", "couldnt close socket!",
					strerror(errno), ERR_CRIT);
		queue_pop(server->queue);
	}
	queue_drop(server->queue, free);
	spin_down_threads(server->workers_count, server->workers);
	spin_down_threads(1, &server->master);
	free(server);
}

t_server		*server_initialise(t_network *network)
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

		server->socket = socket(AF_INET, SOCK_STREAM, 0);
		if (server->socket < 0)
		{
			handle_error("initialise_server", "couldnt open socket!",
					strerror(errno), ERR_CRIT);
			free(server->workers);
			queue_drop(server->queue, free);
			free(server);
			return(NULL);
		}
		if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		{
			handle_error("initialise_server", "couldnt set socket options!",
					strerror(errno), ERR_CRIT);
			queue_drop(server->queue, free);
			close(server->socket);
			free(server->workers);
			free(server);
			return(NULL);
		}

		server->address.sin_family = AF_INET;
		server->address.sin_port = htons(NT_PORT);
		server->address.sin_addr.s_addr = htonl(INADDR_ANY);
		memset(&server->address.sin_zero, 0, sizeof(server->address.sin_zero));
		network->server = server;

		if (bind(server->socket, (struct sockaddr *)&server->address,
					sizeof(server->address)) < 0)
		{
			handle_error("initialise_server", "couldn't bind to socket!",
					strerror(errno), ERR_CRIT);
			close(server->socket);
			queue_drop(server->queue, free);
			free(server->workers);
			free(server);
			return(NULL);
		}

		server->state = NT_STATE_READY;
		server->workers_count = NT_WORKERS_DEF;
		server->workers = spin_up_threads(NT_WORKERS_DEF, NT_WORKERS_MAX, 0, worker_requests, (void *)network);
		if (!server->workers)
		{
			handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
			queue_drop(server->queue, free);
			free(server);
			return(NULL);
		}
		server->master = spin_up_thread(0, worker_incoming, (void *)network);
		if (!server->master)
		{
			handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
			spin_down_threads(server->workers_count, server->workers);
			queue_drop(server->queue, free);
			free(server);
			return(NULL);
		}
	}
	else
		handle_error("initialise_server", strerror(errno), NULL, ERR_CRIT);
	return (server);
}
