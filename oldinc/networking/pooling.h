/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:27:20 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:30:11 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOLING_H
# define POOLING_H

#include "networking/node.h"

typedef struct			s_pool
{
	int					supernodecount;
	t_node				**supernodes;
	t_node				**nodes;
	int					nodecount;
}						t_pool;

t_node				*is_client_a_node(t_pool *pool, t_node *node,
						char *ipv4_addr_str, char *ipv6_addr_str);
t_node				*is_client_a_supernode(t_pool *pool, t_node *node,
						char *ipv4_addr_str, char *ipv6_addr_str);
t_node				*is_client_in_pool(t_pool *pool, t_node *node);
int					pool_add_node(t_pool *pool, t_node *node);
int					pool_add_supernode(t_pool *pool, t_node *node);
void				pool_destroy(t_pool *pool);
t_pool				*pool_initialise(int carriers, int nodes);

#endif
