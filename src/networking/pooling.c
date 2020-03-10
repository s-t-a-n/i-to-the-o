/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/03 22:00:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:38:19 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "networking/constants.h"
#include "networking/pooling.h"
#include "networking/node.h"
#include "networking/network.h"

void	pool_destroy(t_pool *pool)
{
	free(pool->supernodes);
	free(pool->nodes);
	free(pool);
}

t_node		*is_client_a_node(t_pool *pool, t_node *node, char *ipv4_addr_str, char *ipv6_addr_str)
{
	// probably want to do authentication here
	// for now, ip checking is considered 'authentication'

	for (int i = 0; i < pool->nodecount; i++)
	{
		if (pool->nodes[i])
		{
			// lock must be initialized
			if (pthread_mutex_trylock(&pool->nodes[i]->lock) == 0)
			{
				LOG_DEBUG("Thread %d : %s : %s : %s : %s : %s\n", (int)pthread_self(), "is_client_known", "incoming client has ip4", ipv4_addr_str, ", ipv6: ", ipv6_addr_str);

				if (strncmp(pool->nodes[i]->ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
					return(pool->nodes[i]);
				else if (strncmp(pool->nodes[i]->ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
					return(pool->nodes[i]);
			}
		}
	}
	return(NULL);
	node = NULL; //boilerplate
}

t_node		*is_client_a_supernode(t_pool *pool, t_node *node, char *ipv4_addr_str, char *ipv6_addr_str)
{
	for (int i = 0; i < pool->supernodecount; i++)
	{
		if (pool->supernodes[i])
		{
			// lock must be initialized
			if (pthread_mutex_trylock(&pool->supernodes[i]->lock) == 0)
			{
				LOG_DEBUG("Thread %d : %s : %s : %s : %s : %s\n", (int)pthread_self(), "is_client_known", "incoming client has ip4", ipv4_addr_str, ", ipv6: ", ipv6_addr_str);

				if (strncmp(pool->supernodes[i]->ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
					return(pool->supernodes[i]);
				else if (strncmp(pool->supernodes[i]->ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
					return(pool->supernodes[i]);
			}
			else
			{
				printf("no fun with lock  : %i\n", pthread_mutex_trylock(&pool->supernodes[i]->lock));
			}
		}
	}
	return(NULL);
	node = NULL; //boilerplate
}

t_node		*is_client_in_pool(t_pool *pool, t_node *node)
{
	t_node *known_node;
	char *ipv4_addr_str = get_ipv4_str(&node->sockaddr_in);
	char *ipv6_addr_str = get_ipv6_str(&node->sockaddr_in);

	if ((known_node = is_client_a_node(pool, node, ipv4_addr_str, ipv6_addr_str)))
		return (known_node);
	else if ((known_node = is_client_a_supernode(pool, node, ipv4_addr_str, ipv6_addr_str)))
		return (known_node);
	else
		return (NULL);
}

int		pool_add_node(t_pool *pool, t_node *node)
{
	LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_node", "adding node to pool!");
	if (pool->nodecount < POOL_NODE_COUNT)
	{
		node->ipv4_addr = get_ipv4_str(&node->sockaddr_in);
		node->ipv6_addr = get_ipv6_str(&node->sockaddr_in);
		node->container_in = container_create(CONTAINER_MEMCAP_DEF, &node->addrinfo, node->socketfd, 0);
		pool->nodes[pool->nodecount] = node;
		pthread_mutex_init(&node->lock, 0);
		pool->nodecount++;
		return (1);
	}
	else
	{
		LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_node", "Pool is full!");
		node_shutdown(node);
		return (-1);
	}
	return (-1);
}

int		pool_add_supernode(t_pool *pool, t_node *node)
{
	LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_supernode", "adding supernode to pool!");
	if (pool->supernodecount < POOL_SNODE_COUNT)
	{
		int index = (pool->supernodecount == 0) ? 0 : pool->supernodecount - 1;
		pool->supernodes[index] = node;
		pool->supernodecount++;
		return (1);
	}
	else
	{
		LOG_DEBUG("Thread %d : %s : %s\n", (int)pthread_self(), "pool_add_supernode", "Pool is full!");
		node_shutdown(node);
		return (-1);
	}
	return (-1);
}

t_pool	*pool_initialise(int supernodecount, int nodecount)
{
	t_pool	*pool;

	pool = malloc(sizeof(t_pool));
	if (pool)
	{
		pool->supernodes = malloc(sizeof(t_node) * supernodecount);
		if (!pool->supernodes)
		{
			free(pool);
			handle_error("pool_initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
		else
		{
			for (int i = 0; i < supernodecount; i++)
			{
				pool->supernodes[i] = NULL;
			}
		}
		pool->nodes = malloc(sizeof(t_node) * nodecount);
		if (!pool->nodes)
		{
			free(pool);
			free(pool->supernodes);
			handle_error("pool initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
		else
		{
			for (int i = 0; i < nodecount; i++)
				pool->nodes[i] = NULL;
		}
		pool->nodecount = 0;
		pool->supernodecount = 0;
	}
	return(pool);
}
