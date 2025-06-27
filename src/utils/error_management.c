/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:33:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:48 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	perror_free_gc(const char *message)
{
	perror(message);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(EXIT_FAILURE);
}

void	free_gc_exit(void)
{
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(EXIT_FAILURE);
}
