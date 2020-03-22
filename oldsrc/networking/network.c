/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   network.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/09 17:14:21 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 22:48:05 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ito_internal.h"
#include "networking/constants.h"
#include "networking/network.h"

t_network	*network_initialise(void)
{
	t_network *network;

	network = malloc(sizeof(t_network));
	if (network)
	{
		network->pool = pool_initialise(POOL_SNODE_COUNT, POOL_NODE_COUNT);
		if (!network->pool)
		{
			handle_error("network_initialise", "failed to initialise pool!", NULL, ERR_CRIT);
			free(network);
			return (NULL);
		}
		network->server = server_initialise(network);
		if (!network->server)
		{
			handle_error("network_initialise", "failed to initialise server!", NULL, ERR_CRIT);
			pool_destroy(network->pool);
			free(network);
			return (NULL);
		}
		network->client = client_initialise();
		if (!network->client)
		{
			handle_error("network_initialise", "failed to initialise client!", NULL, ERR_CRIT);
			pool_destroy(network->pool);
			server_shutdown(network->server);
			free(network);
			return (NULL);
		}
	}
	else
		handle_error("network_initialise", strerror(errno), NULL, ERR_CRIT);
	return (network);
}
