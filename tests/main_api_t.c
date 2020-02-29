/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_api_t.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/26 20:34:29 by sverschu      #+#    #+#                 */
/*   Updated: 2020/02/29 18:15:20 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

#include "ito.h"
#include "ito_internal.h"

typedef struct		s_data
{
	double			x;
	double			y;
	char			*name;
	int				id;
	int				flags;
	unsigned long long			fatty;
	long long			sfatty;
}					t_data;	

void		dump_package(t_package *package)
{
	size_t	index = 0;
	int	ctr = 0;
	while (index < package->index)
	{
		printf("elem[%i] : |%s|\n", ctr, (char *)&package->mem[index]);
		index += strlen((char *)&package->mem[index]) + 1;
		ctr++;
	}
}

int		main(int argc, char **argv)
{
	t_package package;
	t_data		s_send;
	t_data		s_recv;

	s_send.x = 5.5;
	s_send.y = 9.4;
	s_send.name = "wara waka";
	s_send.id = 42;
	s_send.flags = 1<<4 | 1<<2 | 1<<18;
	s_send.fatty = ULLONG_MAX;
	s_send.sfatty = LLONG_MAX;

	ito_compile_package(&package, "%f%f%s%i%i%llu%lli", s_send.x, s_send.y, s_send.name, s_send.id, s_send.flags, s_send.fatty, s_send.sfatty);
	dump_package(&package);

	// U NO ERRROR HANDLE?
	package.index = 0;
	s_recv.x = *(double *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.y = *(double *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.name = (char *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.id = *(int *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.flags = *(int *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.fatty = *(unsigned long long *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	s_recv.sfatty = *(long long *)ito_decompile_package(&package, "%f%f%s%i%i%llu%lli");
	ito_compile_package(&package, "%f%f%s%i%i%llu%lli", s_recv.x, s_recv.y, s_recv.name, s_recv.id, s_recv.flags, s_send.fatty, s_send.sfatty);
	dump_package(&package);

	free(package.mem);

	return (0);
	argc = 0;
	argv = NULL;
}
