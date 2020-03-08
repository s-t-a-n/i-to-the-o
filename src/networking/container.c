/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:37:05 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:04:06 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/networking.h"
#include "common/memvector1.h"

void			container_destroy(t_container *container)
{
	mvector1_destroy(container->vector);
	freeaddrinfo(container->addrinfo);
	free(container);
}

t_container		*container_create(size_t cap, struct addrinfo *addrinfo,
					int socketfd, uint32_t flags)
{
	t_container	*container;

	container = malloc(sizeof(t_container));
	if (container)
	{
		container->vector = mvector1_init(cap);
		if (container->vector)
		{
			container->addrinfo = addrinfo;
			container->socketfd = socketfd;
			container->flags = flags;
		}
		else
			handle_error("container_create", strerror(errno), NULL, ERR_CRIT);
	}
	else
		handle_error("container_create", strerror(errno), NULL, ERR_CRIT);
	return (container);
}

char			*container_get_data_start(t_container *container)
{
	return((char *)container->vector->mem + FRAME_HEADER_LEN);
}
