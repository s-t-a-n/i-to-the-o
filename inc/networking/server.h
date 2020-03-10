/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 14:22:34 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:40:02 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "networking/queue.h"

typedef struct				s_network t_network;
typedef struct				s_node t_node;

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

/*
** server.c
*/
t_server					*server_initialise(t_network *network);
void						server_shutdown(t_server *server);

/*
** server_processing.c
*/
int							process_package_rq(t_node *node,
								unsigned char *frameheader, t_network *network);
int							process_join_rq(t_node *node,
								unsigned char *frameheader, t_network *network);

#endif
