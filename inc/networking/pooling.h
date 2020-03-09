/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:27:20 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 17:26:17 by sverschu      ########   odam.nl         */
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

# define POOL_RELAY_COUNT	5
# define POOL_MEMBR_COUNT	5

t_conscript					*is_client_known(t_pool *pool, t_conscript *conscript);
t_pool						*pool_initialise(int carriers, int members);
int							pool_add_member(t_pool *pool, t_conscript *conscript);
int							pool_add_relay(t_pool *pool, t_conscript *conscript);

#endif
