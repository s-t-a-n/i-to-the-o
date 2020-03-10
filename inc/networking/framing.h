/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   framing.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 17:55:55 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 19:55:10 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAMING_H
# define FRAMING_H

# include <string.h>
# include <stdint.h>
# include "networking/container.h"

/*
** FRAMING CONCEPT:
**
**	[FRAME_HEADER_SIG]
**	[FRAME_HEADER_ROUTING_DATA]
**	[FRAME_HEADER_REQTYPE]
**	[BODY]
**	[FRAME_FOOTER_SIG]
**
*/

typedef enum							e_request_type
{
	ILLEGAL,
	JOIN,
	PACKAGE
}										t_request_type;

typedef union							u_char_as_uint32
{
	unsigned char c[4];
	uint32_t u;
}										t_char_as_uint32;

int										frame_validate_signature(unsigned char *frameheader);
t_request_type							frame_read_reqtype(unsigned char *frameheader);
int										frame_insert(t_container *container, t_request_type reqtype);
int										frame_detect_footer_signature(const unsigned char *sig, const unsigned char *buffer, int len);
#endif
