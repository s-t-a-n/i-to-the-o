/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 18:38:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:36:54 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/node.h"

void			node_destroy(t_node *node)
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
