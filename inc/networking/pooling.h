/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:27:20 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 16:16:20 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOLING_H
# define POOLING_H

#include "networking/conscript.h"

typedef struct				s_pool
{
	int						relaycount;
	t_conscript				**relays;
	t_conscript				**members;
	int						membercount;
}							t_pool;

t_conscript					*is_client_a_member(t_pool *pool,
								t_conscript *conscript, char *ipv4_addr_str,
								char *ipv6_addr_str);
t_conscript					*is_client_a_relay(t_pool *pool,
								t_conscript *conscript, char *ipv4_addr_str,
								char *ipv6_addr_str);
t_conscript					*is_client_in_pool(t_pool *pool,
								t_conscript *conscript);
t_pool						*pool_initialise(int carriers, int members);
int							pool_add_member(t_pool *pool,
								t_conscript *conscript);
int							pool_add_relay(t_pool *pool,
								t_conscript *conscript);

#endif
