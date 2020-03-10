/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nodestack.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 21:55:40 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 21:55:47 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef NODESTACK_H
# define NODESTACK_H

typedef struct		s_node t_node;

typedef struct		s_nodestack
{
	t_node			*nodes;
	int				count;
}					t_nodestack;

#endif
