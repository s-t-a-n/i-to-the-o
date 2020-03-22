/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   framing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/05 19:23:07 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/11 19:54:52 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "networking/constants.h"
#include "networking/framing.h"
#include "networking/requests.h"

/*
** frame len should be validated BEFORE calling frame_read_type
*/

// this needs some more love
t_request_type	frame_read_request_type(unsigned char *frameheader)
{
	t_char_as_uint32	chint;
	
	frameheader += FRAME_HEADER_SIG_LEN;

	frameheader += FRAME_HEADER_ROUTINGDATA_LEN;

	memcpy(chint.c, frameheader, 4);
	return(chint.u);
};

int				frame_validate_signature(unsigned char *frameheader)
{
	t_char_as_uint32	chint;
	
	memcpy(chint.c, frameheader, FRAME_HEADER_SIG_LEN);
	if (chint.u != FRAME_HEADER_SIG)
		return (0);
	else
		return (1);
}

int				frame_detect_footer_signature(const unsigned char *sig,
					const unsigned char *buffer, int len)
{
	int					i = 0;

	while (i + FRAME_FOOTER_SIG_LEN < len + 1)
	{
		if (memcmp(sig, &buffer[i], FRAME_FOOTER_SIG_LEN) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int				frame_insert(t_container *container, t_request_type reqtype)
{
	int					ctr = 0;
	t_char_as_uint32	chint;
	
	/* insert package signature */
	chint.u = (uint32_t)FRAME_HEADER_SIG;
	memcpy(&container->vector->mem[ctr], chint.c, 4);
	ctr += FRAME_HEADER_SIG_LEN;

	/* insert routing data */
	memset(&container->vector->mem[ctr], 0, FRAME_HEADER_ROUTINGDATA_LEN);
	ctr += FRAME_HEADER_ROUTINGDATA_LEN;

	/* insert request type */
	chint.u = (uint32_t)reqtype;
	memcpy(&container->vector->mem[ctr], chint.c, 4);
	ctr += FRAME_HEADER_REQTYPE_LEN;

	/* other frame data ? */

	/* insert footer after index*/
	chint.u = (uint32_t)FRAME_FOOTER_SIG;
	mvector1_pushback(&container->vector, chint.c, 4);
	return (1);
}
