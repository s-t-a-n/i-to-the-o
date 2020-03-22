/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   requests.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 23:04:51 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:44:21 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

#include "ito_internal.h"
#include "packaging/packaging.h"
#include "networking/client.h"
#include "networking/framing.h"
#include "networking/requests.h"
#include "networking/container.h"
#include "networking/pooling.h"
#include "networking/queue.h"
#include "networking/network.h"
#include "networking/constants.h"

static int	request_package(t_client *client, t_package *package)
{
	client = NULL;
	package = NULL;
	return (-1);
}

static int	request_join(t_client *client, char *ipv4)
{
	t_container			*container;
	struct addrinfo		*info;


	/* open connection to client */
	info = conv_to_addrinfo(ipv4, NT_PORT);
	//free(ipv4);
	if (info)
	{
		int descriptor = open_connection_sync(info);
		if (descriptor >= 0)
		{
			container = container_create(FRAME_HEADER_LEN + FRAME_FOOTER_LEN, info, descriptor, 0);
			if (container)
			{
				frame_insert(container, JOIN);
				queue_safe_add(client->queue, (void *)container);
				return (1);
			}
			else
			{
				handle_error("request_join", strerror(errno), NULL, ERR_CRIT);
				close(descriptor);
				return (-1);
			}
		}
		else
		{
			handle_error("request_join", "couldn't connect to server!", NULL, ERR_WARN);
			//freeaddrinfo(info);
			return (-1);
		}
	}
	else
	{
		handle_error("reqquest_join", strerror(errno), NULL, ERR_WARN);
		freeaddrinfo(info);
		return (-1);
	}
}

int			client_send_request(t_client *client, t_request_type reqtype, void *arg)
{
	switch (reqtype)
	{
		case JOIN:
			return(request_join(client, (char *)arg));
		case PACKAGE:
			return(request_package(client, (t_package *)arg));
		default:
			handle_error("client_send_request", "unkown request type!", NULL, ERR_CRIT);
	}
	return (-1);
}
