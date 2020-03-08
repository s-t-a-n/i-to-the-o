/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   conscript.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:28:53 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 00:05:24 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSCRIPT_H
# define CONSCRIPT_H

# include <pthread.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include "networking/container.h"

typedef struct				s_conscript
{
	struct addrinfo			*addrinfo;
	struct sockaddr			sockaddr;
	socklen_t				socklen;
	char					*ipv4_addr;
	char					*ipv6_addr;
	int						socketfd;
	pthread_mutex_t			lock;
	t_container				container_in;
	t_container				container_out;
}							t_conscript;

#endif
