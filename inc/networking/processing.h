/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   processing.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: sverschu <sverschu@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/11 19:32:04 by sverschu      #+#    #+#                 */
/*   Updated: 2020/03/11 19:32:34 by sverschu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESSING_H
# define PROCESSING_H

void		server_process_request(t_network *network, t_node *node,
				t_request_type request_type);

#endif
