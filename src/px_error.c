/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:40:09 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/02 14:25:39 by kchiang          ###   ########.fr       */
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
	ft_putendl_fd(str, 2);
	exit(EXIT_FAILURE);
}

void	px_free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	return ;
}
