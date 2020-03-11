/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_processing.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 14:22:02 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:38:25 by sverschu      ########   odam.nl         */
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
#include "networking/pooling.h"


/* redo the below in symmetry with client & ev_type */

static int		listen_for_further_packages(t_node *node, t_network *network)
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
			LOG_DEBUG("Server : Thread %d : %s : %s %s\n", (int)pthread_self(), "listen_for_further_requests", "connection closed for", node->ipv4_addr);
			return (-1);
		}
		else if (bytes_read_actual < 0)
		{
			handle_error("listen_for_further_package", "error whilst reading from", node->ipv4_addr, ERR_WARN);
			return (-1);
		}
		else
			bytes_read += bytes_read_actual;

		// in case client becomes funnny
		if (bytes_read > CONTAINER_MEMCAP_MAX)
		{
			handle_error("listen_for_further_package", "client is overflowing server!", node->ipv4_addr, ERR_WARN);
			return (-1);
		}

		mvector1_pushback(&node->container_in->vector, buffer, bytes_read_actual);
	}

	dump_container(node->container_in);
	node->container_in->vector->index = 0;
	return (1);
 	network = NULL;
}

int		process_package_rq(t_node *node, unsigned char *frameheader, t_network *network)
{
	t_node *node_intable;

	LOG_DEBUG("Server : Thread %d : %s : %s : %i\n", (int)pthread_self(), "process_package_rq", "handling request for package!", node->socketfd);

	// if is_client_know returns non-null -> mutex  is locked!
	node_intable = is_client_in_pool(network->pool, node);
	if (node_intable)
	{
		// listen to further packages here
		LOG_DEBUG("Server : Thread %d : %s : %s\n", (int)pthread_self(), "process_package_rq", "client is known!");

		t_container *container = node_intable->container_in;
		container->vector->index = 0;
		if (!mvector1_pushback(&container->vector, frameheader, FRAME_HEADER_LEN))
		{
			handle_error("process_package_rq", strerror(errno), NULL, ERR_CRIT);
			return(-1);
		}

		// stay listening to client here
		while (network->server->state == NT_STATE_READY)
		{
			if (listen_for_further_packages(node, network) < 0)
				break;
		}
		pthread_mutex_unlock(&node->lock);
		return(1);
	}
	else
		handle_error("process_package_rq", " unknown client is sending requests to server!", NULL, ERR_WARN);
	return (-1);
}

int		process_join_rq(t_node *node, unsigned char *frameheader, t_network *network)
{
	LOG_DEBUG("Server : Thread %d : %s : %s : %i\n", (int)pthread_self(), "process_join_rq", "handling request to join!", node->socketfd);
	// no authenthication for now
	if (pool_add_node(network->pool, node))
	{
		// start listening to further requests
		process_package_rq(node, frameheader, network);
		return (1);
	}
	else
		return (-1);
	frameheader = NULL; //stub
}
