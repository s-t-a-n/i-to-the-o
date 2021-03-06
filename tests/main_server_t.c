/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_server_t.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/02 21:19:15 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 15:33:58 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "networking/server.h"
#include "networking/network.h"
#include "networking/constants.h"
#include "ito_internal.h"

int	main(void)
{
	t_network	*network;

	network = network_initialise();
	sleep(60);

	LOG_DEBUG("%s\n", "stopping server!");
	network->server->state = NT_STATE_STOP;

	sleep(10);
	server_shutdown(network->server);
}
