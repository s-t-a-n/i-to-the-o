/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:38:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:37:47 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/node.h"

// needs a heck more love
void			node_shutdown(t_node *node)
{
	pthread_mutex_destroy(&node->lock);
	free(node);
}

t_node		*node_initiate(struct sockaddr_in sockaddr_in, socklen_t socklen)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (node)
	{
		pthread_mutex_init(&node->lock, 0);
		//node->addrinfo = (struct addrinfo)0;
		node->sockaddr_in = sockaddr_in;
		node->socklen = socklen;
		node->ipv4_addr = NULL;
		node->ipv6_addr = NULL;
		node->socketfd = -1;
	}
	return(node);
}
