/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:37:05 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 19:58:09 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ito_internal.h"
#include "networking/constants.h"
#include "networking/container.h"
#include "networking/framing.h"
#include "common/memvector1.h"

int				container_insert_package(t_package *package,
					t_container *container)
{
	return (mvector1_pushback(&container->vector, package->mem, package->index)); 
}

void			container_destroy(t_container *container)
{
	mvector1_destroy(container->vector);
	freeaddrinfo(container->addrinfo);
	free(container);
}

t_container		*container_create(size_t cap, int socketfd, uint32_t flags)
{
	t_container	*container;

	container = malloc(sizeof(t_container));
	if (container)
	{
		container->vector = mvector1_init(cap);
		if (container->vector)
		{
			container->vector->index = FRAME_HEADER_LEN;
			container->addrinfo = NULL;
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

void					dump_container(t_container *container)
{
	printf("CONTAINER: --------------------\n");
	printf("index : %li\n", container->vector->index);
	printf("cap : %li\n", container->vector->cap);
	for (int i = 0; i < container->vector->index; i++)
	{
		if (container->vector->mem[i] == '\0')
			printf("\\0");
		else
			printf("%c", container->vector->mem[i]);
	}
	printf("\n------------------- :CONTAINER\n");
}
