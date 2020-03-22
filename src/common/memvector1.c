/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mvector1.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 19:20:23 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:05:28 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "common/memvector1.h"

void				mvector1_destroy(t_mvector1 *vec)
{
	free(vec->mem);
	free(vec);
}

t_mvector1		*mvector1_init(size_t cap)
{
	t_mvector1 	*vec;

	vec = (t_mvector1 *)malloc(sizeof(t_mvector1));
	if (vec)
	{
		vec->mem = (unsigned char *)malloc(cap + 1);
		if (!(vec->mem))
		{
			free(vec);
			return (NULL);
		}
		vec->cap = cap;
		vec->index = 0;
	}
	return (vec);
}

t_mvector1		*mvector1_grow(t_mvector1 *old_vec)
{
	t_mvector1	*new_vec;

	new_vec = mvector1_init(old_vec->cap * 2);
	if (new_vec)
	{
		while ((size_t)new_vec->index < old_vec->cap)
		{
			(new_vec->mem)[new_vec->index] = old_vec->mem[new_vec->index];
			(new_vec->index)++;
		}
		mvector1_destroy(old_vec);
	}
	return (new_vec);
}

int					mvector1_pushback(t_mvector1 **vec, unsigned char *element,
						size_t len)
{
	if ((*vec)->index + len >= (*vec)->cap)
		*vec = mvector1_grow(*vec);
	if (*vec)
	{
		for (size_t i = 0; i < len; i++)
		{
			((*vec)->mem)[(*vec)->index] = element[i];
			((*vec)->index)++;
		}
		return (1);
	}
	return (0);
}
