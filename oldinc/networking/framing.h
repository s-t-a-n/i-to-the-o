/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   framing.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/08 17:55:55 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/11 20:56:57 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRAMING_H
# define FRAMING_H

# include <string.h>
# include <stdint.h>
# include "networking/container.h"

// to do: get rid of signature entirely; this needs to be handles by authenthication

/*
** FRAMING CONCEPT:
**
**	[FRAME_HEADER_ROUTING_DATA]
**	[FRAME_HEADER_REQTYPE]
**  [FRAME_HEADER_BODY_SPEC]
**	[FRAME_BODY]
**
*/

typedef enum			e_request_type t_request_type;

typedef union			u_char_as_uint32
{
	unsigned char c[4];
	uint32_t u;
}						t_char_as_uint32;

int						frame_validate_signature(unsigned char *frameheader);
t_request_type			frame_read_request_type(unsigned char *frameheader);
int						frame_insert(t_container *container,
							t_request_type request_type);
int						frame_detect_footer_signature(const unsigned char *sig,
							const unsigned char *buffer, int len);
#endif
