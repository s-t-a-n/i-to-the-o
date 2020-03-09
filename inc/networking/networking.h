/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   networking.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 21:43:08 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 17:26:42 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKING_H
# define NETWORKING_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "ito_internal.h"
#include "common/memvector1.h"
#include "common/threading.h"
#include "common/sleep.h"
#include "networking/framing.h"
#include "networking/queue.h"
#include "networking/container.h"
#include "networking/pooling.h"
#include "conscript.h"

/*
** network defines
*/

# define NT_PORT			4200

# define NT_QUEUE_CAP		1024 // maximum number of pending requests
# define NT_QUEUE_BACKLOG	10

# define NT_WORKERS_DEF		5  // an extra thread is spawned for handling incoming
# define NT_WORKERS_MAX		20 // maximum number of threads to spin up

# define NT_RECV_BUFSIZE	1024
# define NT_SEND_BUFSIZE	1024

# define NT_CON_INIT		0x01
# define NT_CON_EXIT		0x02
# define NT_CON_PACK		0x04

# define NT_STATE_READY		1
# define NT_STATE_STOP		2

# define NT_WRITE_DELAY		50	// time in microseconds that the thread holds before attempting to write again
# define NT_READ_DELAY		50
# define NT_WRITE_MAXRETRY	10	// max amount of times write may come back zero
# define NT_READ_MAXRETRY	10

typedef struct				s_server
{
	struct sockaddr_in		address;
	int						socket;
	pthread_t				master;
	pthread_t				*workers;
	int						workers_count;
	t_queue					*queue;
	int						state;
}							t_server;

typedef struct				s_client
{
	pthread_t				master;
	pthread_t				*workers;
	int						workers_count;
	t_queue					*queue;
	int						state;
}							t_client;

typedef struct				s_network
{
	t_server				*server;
	t_client				*client;
	t_conscript				*memberstack;
	t_pool					*pool;
}							t_network;

/*
** common.c
*/
struct addrinfo				*conv_to_addrinfo(char *hostname, int port);
int							open_connection_sync(struct addrinfo *info);

/*
** server.c
*/
t_server					*server_initialise(t_network *network);
void						server_shutdown(t_server *server);

/*
** client.c
*/
t_client					*client_initialise(void);
void						client_shutdown(t_client *client);

/*
** network.c
*/
t_network					*network_initialise(void);
#endif
