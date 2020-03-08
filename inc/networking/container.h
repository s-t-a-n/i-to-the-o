/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 19:45:44 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:33:21 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINER_H
# define CONTAINER_H

# include "common/memvector1.h"

typedef struct				s_container
{
	struct addrinfo			*addrinfo;
	int						socketfd;
	t_mvector1				*vector;
	uint32_t				flags;
}							t_container;

# define CONTAINER_MEMCAP_DEF	512
# define CONTAINER_MEMCAP_MAX	2048

void				container_destroy(t_container *container);
t_container			*container_create(size_t cap, struct addrinfo *addrinfo,
						int socketfd, uint32_t flags);

#endif
