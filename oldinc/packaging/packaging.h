/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   packaging.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 21:44:17 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 20:06:46 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKAGING_H
# define PACKAGING_H

#include <stddef.h>
#include <sys/types.h>
#include <stdarg.h>

#include "common/memvector1.h"

/* dont allow packages bigger than MAX_PACKAGE_SIZE (def: 4KB) */
# define PACKAGE_MAX_SIZE 4096

/* estimated size of packages */
# define PACKAGE_DEF_SIZE 512

typedef struct				s_mvector1 t_package;

/*
** package_compilation.c
*/
int			add_data_to_package(va_list *args, const char * restrict formatstr, t_package *package);

/*
** package_decompilation.c
*/
int			get_data_from_package(va_list *args, const char * restrict formatstr, t_package *package);

/*
** package.c
*/
t_package	*package_initialise(void);
void		package_destroy(t_package *package);
void		dump_package(t_package *package);

#endif
