/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_cmd_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 14:41:45 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/22 14:53:49 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_free_all_exit(char *execpath, char **cmd, int exit_code);

void	px_cmd_error(char *execpath, char **cmd)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (cmd[0] && ft_strchr(cmd[0], '/'))
	{
		perror(NULL);
		if (access(cmd[0], F_OK) == -1)
			px_free_all_exit(execpath, cmd, NOT_FOUND);
		else if (access(cmd[0], X_OK) == -1)
			px_free_all_exit(execpath, cmd, PERM_DENIED);
	}
	if (access(execpath, F_OK) == -1)
	{
		ft_putendl_fd("Command not found", STDERR_FILENO);
		px_free_all_exit(execpath, cmd, NOT_FOUND);
	}
	else if (access(execpath, X_OK) == -1)
	{
		perror(NULL);
		px_free_all_exit(execpath, cmd, PERM_DENIED);
	}
}

static void	px_free_all_exit(char *execpath, char **cmd, int exit_code)
{
	free(execpath);
	px_free_arg(cmd);
	exit(exit_code);
}
