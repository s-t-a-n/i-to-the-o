/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   network.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/09 17:14:21 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 15:31:17 by sverschu      ########   odam.nl         */
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
		network->pool = pool_initialise(POOL_RELAY_COUNT, POOL_MEMBR_COUNT);
		network->server = server_initialise(network);
	}
	else
		handle_error("network_initialise", strerror(errno), NULL, ERR_CRIT);
	return (network);
}
