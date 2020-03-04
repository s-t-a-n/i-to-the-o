/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   common.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 17:31:44 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 23:37:50 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

struct addrinfo		*conv_to_addrinfo(char *hostname, int port)
{
	char			service[8];
	struct addrinfo	*info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_DEFAULT;
	
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
			if (setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1},
						sizeof(int)) < 0)
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
