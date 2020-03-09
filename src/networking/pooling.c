/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/03 22:00:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 00:33:56 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/networking.h"
#include "networking/pooling.h"

void	pool_destroy(t_pool *pool)
{
	free(pool->carriers);
	free(pool->members);
	free(pool);
}

t_conscript		*is_client_known(t_pool *pool, t_conscript *conscript)
{
	// probably want to do authentication here
	// for now, ip checking is considered 'authentication'

	return (NULL);
	for (int i = 0; i < pool->members_count; i++)
	{
		// lock must be initialized
		if (pthread_mutex_trylock(&pool->members[i].lock) == 0)
		{
			struct sockaddr_in	*ipv4_addr = (struct sockaddr_in *)&conscript->sockaddr_in;
			char ipv4_addr_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, (struct in_addr *)&ipv4_addr->sin_addr, ipv4_addr_str, INET_ADDRSTRLEN);

			struct sockaddr_in6	*ipv6_addr = (struct sockaddr_in6 *)&conscript->sockaddr_in;
			char ipv6_addr_str[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, (struct in_addr *)&ipv6_addr->sin6_addr, ipv6_addr_str, INET6_ADDRSTRLEN);
			if (strncmp(pool->members[i].ipv4_addr, ipv4_addr_str, INET_ADDRSTRLEN) == 0)
				return(&pool->members[i]);
			else if (strncmp(pool->members[i].ipv6_addr, ipv6_addr_str, INET6_ADDRSTRLEN) == 0)
				return(&pool->members[i]);
		}
	}
	return(NULL);
}

t_pool	*pool_initiate(int carriers, int members)
{
	t_pool	*pool;

	pool = malloc(sizeof(t_pool));
	if (pool)
	{
		pool->carriers = malloc(sizeof(t_conscript) * carriers);
		if (!pool->carriers)
		{
			free(pool);
			handle_error("pool initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
		pool->members = malloc(sizeof(t_conscript) * members);
		if (!pool->members)
		{
			free(pool);
			free(pool->carriers);
			handle_error("pool initiate", strerror(errno), NULL, ERR_CRIT);
			return (NULL);
		}
	}
	return(pool);
}
