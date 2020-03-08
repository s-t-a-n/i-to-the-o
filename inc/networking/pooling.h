/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pooling.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 22:27:20 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/09 00:20:57 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOLING_H
# define POOLING_H

#include "networking/conscript.h"

typedef struct				s_pool
{
	t_conscript				*carriers;
	t_conscript				*members;
}							t_pool;

# define POOL_SIZE			5

t_conscript					*is_client_known(t_pool *pool, t_conscript *conscript);
t_pool						*pool_initiate(int carriers, int members);

#endif
