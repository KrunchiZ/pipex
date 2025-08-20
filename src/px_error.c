/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:40:09 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 13:52:11 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	px_perror_free_exit(char *str, pid_t *pid)
{
	free(pid);
	px_perror_exit(str, EXIT_FAILURE);
}

void	px_perror_exit(char *str, int exit_code)
{
	perror(str);
	exit(exit_code);
}

void	px_error_abort(char *str, int exit_code)
{
	ft_putendl_fd(str, STDERR_FILENO);
	exit(exit_code);
}

void	px_free_arg(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		free(arg[i++]);
	free(arg);
	return ;
}
