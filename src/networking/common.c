/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   common.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 17:31:44 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 14:47:41 by sverschu      ########   odam.nl         */
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
#include <stdio.h>

#include "ito_internal.h"
#include "networking/constants.h"

struct addrinfo		*conv_to_addrinfo(char *hostname, int port)
{
	char			service[8];
	struct addrinfo	*info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED;
	
	snprintf(service, 8, "%d", port);

	if (getaddrinfo(hostname, service, &hints, &info) != 0)
	{
		handle_error("conv_to_addrinfo", "couldn't get address info:",
				strerror(errno), ERR_WARN);
		info = NULL;
	} 
	free(hostname);
	return (info);
}

/*
**	synchroniously open connection
**	problem here is that connection could timeout for 30 seconds, clogging up
**  the queue
**  therefore: add async option: https://www.isc.org/blogs/2011-01-19-multi-homed-server-over-tcp/
*/

int					open_connection_sync(struct addrinfo *info)
{
	struct addrinfo		*ai;
	int					descriptor;
	
	for (ai = info; ai != NULL; ai = ai->ai_next)
	{
		descriptor = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (descriptor != -1)
		{
			int error = 0;
			error += setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
			error += setsockopt(descriptor, SOL_SOCKET, SO_KEEPALIVE, &(int){1}, sizeof(int));
			if (error > 0)
			{
				handle_error("open_connection_sync", "couldn't set sock opts:",
						strerror(errno), ERR_WARN);
				close(descriptor);	
				descriptor = -1;
			}

			if (connect(descriptor, ai->ai_addr, ai->ai_addrlen) == 0)
				break;
		}
	}

	freeaddrinfo(info);

	if (ai == NULL)
	{
		handle_error("open_connection_sync", "couldn't connect to host!", NULL,
				ERR_WARN);
		descriptor = -1;
	}
	return (descriptor);
}

char	*get_ipv6_str(struct sockaddr_in *sai)
{
	char					ipv6_addr_str[INET6_ADDRSTRLEN];
	struct sockaddr_in6		*ipv6_sockaddr;
	
	ipv6_sockaddr = (struct sockaddr_in6 *)sai;
	inet_ntop(AF_INET6, (struct in_addr *)&ipv6_sockaddr->sin6_addr, ipv6_addr_str, INET6_ADDRSTRLEN);
	return(strndup(ipv6_addr_str, INET6_ADDRSTRLEN));
}

char	*get_ipv4_str(struct sockaddr_in *sai)
{
	char					ipv4_addr_str[INET6_ADDRSTRLEN];
	struct sockaddr_in		*ipv4_sockaddr;
	
	ipv4_sockaddr = (struct sockaddr_in *)sai;
	inet_ntop(AF_INET, (struct in_addr *)&ipv4_sockaddr->sin_addr, ipv4_addr_str, INET_ADDRSTRLEN);
	return(strndup(ipv4_addr_str, INET_ADDRSTRLEN));
}
