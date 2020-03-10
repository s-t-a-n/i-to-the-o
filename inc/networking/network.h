/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   network.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 14:37:29 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:39:07 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_H
# define NETWORK_H

/*
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
*/

#include "networking/pooling.h"
#include "networking/node.h"
#include "networking/server.h"
#include "networking/client.h"

typedef struct				s_network
{
	t_server				*server;
	t_client				*client;
	t_node				*memberstack;
	t_pool					*pool;
}							t_network;

/*
** common.c
*/
struct addrinfo				*conv_to_addrinfo(char *hostname, int port);
int							open_connection_sync(struct addrinfo *info);
char						*get_ipv6_str(struct sockaddr_in *sockaddr_in);
char						*get_ipv4_str(struct sockaddr_in *sockaddr_in);

/*
** network.c
*/
t_network					*network_initialise(void);
#endif
