/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_api_t.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/26 20:34:29 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/02 21:19:45 by sverschu      ########   odam.nl         */
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

int		main(void)
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

	char *formatstr = "%f%f%s%i%i%llu%lli";
	ito_compile_package(&package, formatstr, s_send.x, s_send.y, s_send.name, s_send.id, s_send.flags, s_send.fatty, s_send.sfatty);
	dump_package(&package);

	ito_decompile_package(&package, formatstr, &s_recv.x, &s_recv.y, &s_recv.name, &s_recv.id, &s_recv.flags, &s_recv.fatty, &s_recv.sfatty);

	ito_compile_package(&package, formatstr, s_recv.x, s_recv.y, s_recv.name, s_recv.id, s_recv.flags, s_send.fatty, s_send.sfatty);
	dump_package(&package);

	free(package.mem);
	free(s_recv.name);
	return (0);
}
