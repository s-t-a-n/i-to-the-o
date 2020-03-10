/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   requests.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/10 23:05:32 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/10 23:27:10 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTS_H
# define REQUESTS_H

typedef struct	s_client t_client;

typedef enum	e_request_type
{
				ILLEGAL,
				JOIN,
				PACKAGE
}				t_request_type;

int				client_send_request(t_client *client, t_request_type reqtype,
					void *arg);

#endif
