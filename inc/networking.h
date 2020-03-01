/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   networking.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 21:43:08 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/01 22:40:03 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKING_H
# define NETWORKING_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "ito_internal.h"

/*
** network defines
*/

# define NT_PORT_RT			4200
# define NT_PORT_DATA		4201

# define NT_WORKERS_DEF		0x05
# define NT_WORKERS_MAX		0x14

# define NT_BUF_SIZE		1024

# define NT_CON_INIT		0x01
# define NT_CON_EXIT		0x02
# define NT_CON_PACK		0x03

typedef struct				s_member
{
	struct in_addr			sin_addr;
	pthread_mutex_t			lock;
	t_package				package;
}							t_member;

typedef struct				s_server
{
	struct sockaddr_in		address;
	pthread_t				*thread_tab;
}							t_server;

#endif
