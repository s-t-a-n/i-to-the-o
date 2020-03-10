/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ito.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/23 19:00:56 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 20:05:30 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITO_H
# define ITO_H

#include <stddef.h>

#include "packaging/packaging.h"

/*
** Include this header to use ITO.
*/

typedef struct				s_ito_specs
{
    char					*ip_address;
    int						pool;
}							t_ito_specs;

typedef struct				s_ito
{
	t_ito_specs				ito_specs;
}							t_ito;

int							ito_decompile_package(t_package *package, const char * restrict formatstr, ...);
t_package					*ito_compile_package(const char * restrict formatstr, ...);
int							ito_send_package(t_ito *ito, t_package *package);

t_ito						*ito_quick_init(char *ip);
t_ito						*ito_full_init(t_ito_specs *ito_specs);
int							ito_shutdown(t_ito *ito);
int							ito_hook(t_ito *ito, int (*f_ptr)(), void *param);

#endif
