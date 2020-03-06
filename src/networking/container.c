/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:37:05 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/06 20:44:23 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking.h"

void			container_destroy(t_container *container)
{
	free(container->mem);
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
		container->vector = nvectir1_init(cap);
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

void			container_insert_frames(t_container *container, uint32_t flags)
{
	container = NULL;
	flags = 0;
}

char			*container_get_data_start(t_container *container)
{
	return((char *)container->vector->mem + NT_FRAME_HEADER_LEN);
}
