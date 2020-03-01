/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   packaging.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/01 21:44:17 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/01 21:47:28 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKAGING_H
# define PACKAGING_H

#include "ito_internal.h"

/*
** package_compilation.c
*/
int		add_data_to_package(va_list *args, const char * restrict formatstr, t_package *package);

/*
** package_decompilation.c
*/
int		get_data_from_package(va_list *args, const char * restrict formatstr, t_package *package);

#endif
