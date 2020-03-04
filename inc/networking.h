/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   networking.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 21:43:08 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/04 19:51:21 by sverschu      ########   odam.nl         */
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

#include <pthread.h>
#include <signal.h>

#include "ito_internal.h"

/*
** network defines
*/

# define NT_PORT			4200
# define NT_PORT_C			"4200"

# define NT_QUEUE_CAP		1024 // maximum number of pending requests
# define NT_QUEUE_BACKLOG	10

# define NT_WORKERS_DEF		5  // an extra thread is spawned for handling incoming
# define NT_WORKERS_MAX		20 // maximum number of threads to spin up

# define NT_BUF_SIZE		1024

# define NT_CON_INIT		0x01
# define NT_CON_EXIT		0x02
# define NT_CON_PACK		0x04

# define NT_STATE_READY		1
# define NT_STATE_STOP		2

typedef struct				s_member
{
	struct in_addr			sin_addr;
	pthread_mutex_t			lock;
	t_package				package;
}							t_member;

typedef struct				s_package_enroute
{
	struct addrinfo			*addrinfo;
	t_package				*package;
}							t_package_enroute;

typedef struct				s_queue
{
	pthread_mutex_t			lock;
	pthread_cond_t			signal;
	int						cap;
	int						size;
	int						front;
	int						back;
	void					**elements;
}							t_queue;

typedef struct				s_server
{
	struct sockaddr_in		address;
	int						socket;
	pthread_t				*thread_tab;
	t_queue					*queue;
	t_member				*members;
	int						state;
}							t_server;

typedef struct				s_client
{
	pthread_t				*thread_tab;
	t_queue					*queue;
	t_member				*members;
	int						state;
}							t_client;

typedef struct				s_pool
{
	t_member				*carriers;
	t_member				*members;
}							t_pool;

typedef struct				s_network
{
	t_server				*server;
	t_client				*client;
	t_pool					*pool;
}							t_network;

/*
** common.c
*/
struct addrinfo				*conv_to_addrinfo(char *hostname, char *service);
int							open_connection_sync(struct addrinfo *info);

/*
** queue.c
*/
void						queue_drop(t_queue *queue);
void						queue_pop(t_queue *queue);
void						queue_push(t_queue *queue, void *element);
void						*queue_peek(t_queue *queue);
void						*queue_safe_get(t_queue *queue);
void						queue_safe_add(t_queue *queue, void *element);
t_queue						*queue_create(int cap);

/*
** server.c
*/
t_server					*initialise_server(void);
void						shutdown_server(t_server *server);

/*
** client.c
*/

t_client					*initialise_client(void);
void						shutdown_client(t_client *client);
#endif
