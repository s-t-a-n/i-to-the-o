/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:28:53 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:36:46 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef node_H
# define node_H

# include <pthread.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include "networking/container.h"

typedef struct				s_node
{
	struct addrinfo			addrinfo;
	struct sockaddr_in		sockaddr_in;
	socklen_t				socklen;
	char					*ipv4_addr;
	char					*ipv6_addr;
	int						socketfd;
	pthread_mutex_t			lock;
	t_container				*container_in;
}							t_node;

t_node						*node_initiate(struct sockaddr_in, socklen_t socklen);
void						node_discharge(t_node *node);
#endif
