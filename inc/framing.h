/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   framing.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 17:55:55 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/08 22:02:41 by sverschu      ########   odam.nl         */
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

# define FRAME_HEADER_SIG				0x424242 // single integer id
# define FRAME_FOOTER_SIG				0x242424

# define FRAME_HEADER_SIG_LEN			4
# define FRAME_HEADER_REQTYPE_LEN		4
# define FRAME_HEADER_ROUTINGDATA_LEN	16 // 4 integer space of routing vars

# define FRAME_FOOTER_SIG_LEN			4

# define FRAME_HEADER_LEN				24 // combination of lengths above	
# define FRAME_FOOTER_LEN				1

int										frame_validate_signature(unsigned char *frame);
t_request_type							frame_read_reqtype(unsigned char *frame);
int										frame_insert(t_container *container, t_request_type reqtype);

#endif
