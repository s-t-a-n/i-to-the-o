/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main_framing_t.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 17:27:27 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 18:24:28 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "limits.h"
#include "networking/constants.h"
#include "networking/network.h"
#include "networking/framing.h"
#include "networking/container.h"
#include "networking/client.h"
#include "packaging/packaging.h"
#include "ito_internal.h"

typedef struct			s_data
{
	double				x;
	double				y;
	char				*name;
	int					id;
	int					flags;
	unsigned long long	fatty;
	long long			sfatty;
}						t_data;

void					test_frame(void)
{
	t_data			s_send;
	t_package		*package;
	t_container		*container;
	t_char_as_uint32	chint;
	chint.u = FRAME_FOOTER_SIG;
	container = container_create(512, NULL, 0, 0);

	s_send.x = 5.5;
	s_send.y = 9.4;
	s_send.name = "wara waka";
	s_send.id = 42;
	s_send.flags = 1<<4 | 1<<2 | 1<<18;
	s_send.fatty = ULLONG_MAX;
	s_send.sfatty = LLONG_MAX;

	const char *formatstr = "%f%f%s%i%i%llu%lli";
	package = ito_compile_package(formatstr, s_send.x, s_send.y, s_send.name, s_send.id, s_send.flags, s_send.fatty, s_send.sfatty);
	dump_package(package);

	container_insert_package(package, container);
	frame_insert(container, JOIN);
	dump_container(container);

	printf("footer signature: %c%c%c%c\n", chint.c[0], chint.c[1], chint.c[2], chint.c[3]);
	for (int i = 0; i < container->vector->index; i++)
		printf("memcmp : %i\n", memcmp(chint.c, &container->vector->mem[i], FRAME_FOOTER_SIG_LEN));
	if (frame_detect_footer_signature(chint.c, container->vector->mem, container->vector->index) < 0)
		handle_error("test_frame", "couldnt detect footer signature!", NULL, ERR_WARN);
}

int						main(void)
{
	test_frame();
}
