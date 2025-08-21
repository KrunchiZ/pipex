/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:40:09 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/21 19:17:45 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	px_closefd_exit(t_vars *vars)
{
	close(vars->pipefd[0]);
	close(vars->pipefd[1]);
	close(vars->outfd);
	exit(EXIT_FAILURE);
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

void	px_free_exit(char **cmd, char *str, int exit_code)
{
	px_free_arg(cmd);
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
