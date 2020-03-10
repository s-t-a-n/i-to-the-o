/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   container.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 19:45:44 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 17:33:45 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINER_H
# define CONTAINER_H

# include "common/memvector1.h"

typedef struct				s_package t_package;

typedef struct				s_container
{
	struct addrinfo			*addrinfo;
	int						socketfd;
	t_mvector1				*vector;
	uint32_t				flags;
}							t_container;

int					container_insert_package(t_package *package,
						t_container *container);
void				container_destroy(t_container *container);
t_container			*container_create(size_t cap, struct addrinfo *addrinfo,
						int socketfd, uint32_t flags);
void				dump_container(t_container *container);
#endif
