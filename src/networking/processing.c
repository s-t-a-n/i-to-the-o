/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   processing.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/11 18:09:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/11 21:06:17 by sverschu      ########   odam.nl         */
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

#include "networking/constants.h"
#include "networking/server.h"
#include "networking/network.h"
#include "networking/node.h"
#include "networking/framing.h"
#include "networking/requests.h"
#include "networking/pooling.h"

/* redo the below in symmetry with client & ev_type */

static int		process_packages(t_node *node)
{
	unsigned char		buffer[NT_RECV_BUFSIZE];
	ssize_t				bytes_read_actual;
	ssize_t				bytes_read;
	t_char_as_uint32 	chint;

	chint.u = FRAME_FOOTER_SIG;

	buffer[0] = 0;
	bytes_read = 0;
	bytes_read_actual = 0;
	while (frame_detect_footer_signature(chint.c, buffer, bytes_read_actual) < 0)
	{
		bytes_read_actual = recv(node->socketfd, buffer, NT_RECV_BUFSIZE, 0);
		if (bytes_read_actual == 0)
		{
			LOG_DEBUG("%s : %s %s\n", "listen_for_further_requests", "connection closed for", node->ipv4_addr);
			return (-1);
		}
		else if (bytes_read_actual < 0)
		{
			handle_error("process_packages", "error whilst reading from", node->ipv4_addr, ERR_WARN);
			return (-1);
		}
		else
			bytes_read += bytes_read_actual;

		// in case client becomes funnny
		if (bytes_read > CONTAINER_MEMCAP_MAX)
		{
			handle_error("process_packages", "client is overflowing server!", node->ipv4_addr, ERR_WARN);
			return (-1);
		}

		mvector1_pushback(&node->container_in->vector, buffer, bytes_read_actual);
	}
	node->container_in->vector->index = 0;
	return (1);
}

static int			process_request_package(t_container *container, t_network *network)
{
	t_node			*node_intable;

	LOG_DEBUG("%s : %s : %i\n", "process_request_package", "handling request for package!", container->socketfd);

	// if is_client_know returns non-null -> mutex  is locked!
	node_intable = is_client_in_pool(network->pool, container);
	if (node_intable)
	{
		// listen to further packages here
		LOG_DEBUG("%s : %s\n", "process_request_package", "client is known!");

		container = node_intable->container_in;
		while (network->server->state == NT_STATE_READY)
		{
			if (process_packages(node) < 0)
				break;
		}
		pthread_mutex_unlock(&node->lock);
		return(1);
	}
	else
		handle_error("process_request_package", " unknown client is sending requests to server!", NULL, ERR_WARN);
	return (-1);
}

// completely redo with basic authenthication
static int		process_request_join(t_container *container, t_network *network)
{
	LOG_DEBUG("%s : %s\n", "process_join_rq", "handling request to join!");
	//create node here from container data
	//return (pool_add_node(network->pool, connection));
}

void	server_process_request(t_network *network, t_container *container, t_request_type request_type)
{
	switch (request_type)
	{
		case JOIN:
			if(!process_request_join(container, network))
				handle_error("server_process_request", "request to join failed!",
						NULL, ERR_WARN);
			return ;
		case PACKAGE:
			if (!process_request_package(container, network))
				handle_error("server_process_request", "request to join failed!",
						NULL, ERR_WARN);
			return ;
		default:
			return ;
	}
}
