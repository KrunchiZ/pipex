/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_perror_exit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:40:09 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 18:01:04 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	px_perror_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	px_error_abort(char *str)
{
	ft_putendl_fd(str);
	exit(EXIT_FAILURE);
}
