/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memvector1.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 19:32:50 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 20:04:04 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMVECTOR1_H
# define MEMVECTOR1_H

#include <sys/types.h>
#include <stdlib.h>

typedef struct				s_mvector1
{
	unsigned char			*mem;
	ssize_t					index;
	size_t					cap;
}							t_mvector1;

/*
** mvector1.c
*/
void						mvector1_destroy(t_mvector1 *vec);
t_mvector1					*mvector1_init(size_t cap);
t_mvector1					*mvector1_grow(t_mvector1 *old_vec);
int							mvector1_pushback(t_mvector1 **vec,
								unsigned char *element, size_t len);

#endif
