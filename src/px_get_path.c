/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 16:04:54 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	px_path_is_valid(char *cmd_path, int only_cmd_zero, char **cmd);
static char	**px_parse_path_env(char **envp, char **cmd, char *cmd_path);
static char	*px_get_next_path(char **paths, char *cmd_path, char **cmd);
static char	*px_join_path(char **paths, char *path, char *cmd_path, char **cmd);

char	*px_get_path(char **cmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;

	if (cmd[0] && px_path_is_valid(cmd[0], true, cmd))
		return (ft_strdup(cmd[0]));
	cmd_path = ft_strjoin("./", cmd[0]);
	if (!cmd_path)
	{
		px_free_arg(cmd);
		px_error_abort("pipex: ft_strjoin: './' + cmd failed", EXIT_FAILURE);
	}
	if (px_path_is_valid(cmd_path, false, cmd))
		return (cmd_path);
	env_paths = px_parse_path_env(envp, cmd, cmd_path);
	return (px_get_next_path(env_paths, cmd_path, cmd));
}

static int	px_path_is_valid(char *cmd_path, int only_cmd_zero, char **cmd)
{
	if (access(cmd_path, F_OK) == 0)
	{
		if (access(cmd_path, X_OK) == 0)
			return (true);
		else
		{
			ft_putstr_fd("pipex: ", STDERR_FILENO);
			ft_putstr_fd(cmd_path, STDERR_FILENO);
			if (!only_cmd_zero)
				free(cmd_path);
			px_free_arg(cmd);
			ft_putstr_fd(": ", STDERR_FILENO);
			px_error_abort(strerror(errno), PERM_DENIED);
		}
	}
	if (only_cmd_zero && (*cmd_path == '/' || !ft_strncmp(cmd_path, "./", 2)))
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(cmd_path, STDERR_FILENO);
		px_free_arg(cmd);
		ft_putstr_fd(": ", STDERR_FILENO);
		px_error_abort(strerror(errno), NOT_FOUND);
	}
	return (false);
}

static char	**px_parse_path_env(char **envp, char **cmd, char *cmd_path)
{
	char	*env_var;
	char	**env_paths;

	while (*envp)
	{
		env_var = *envp++;
		if (!ft_strncmp(env_var, "PATH=", 5))
			break ;
	}
	env_paths = ft_split(env_var, "=:");
	if (!env_paths)
	{
		free(cmd_path);
		px_free_arg(cmd);
		px_error_abort("pipex: ft_split: malloc failed", EXIT_FAILURE);
	}
	return (env_paths);
}

static char	*px_get_next_path(char **paths, char *cmd_path, char **cmd)
{
	char	**cursor;

	cursor = paths;
	while (*cursor)
	{
		cmd_path = px_join_path(paths, *cursor++, cmd_path, cmd);
		if (px_path_is_valid(cmd_path, false, cmd))
		{
			px_free_arg(paths);
			return (cmd_path);
		}
	}
	px_free_arg(paths);
	return (cmd_path);
}

static char	*px_join_path(char **paths, char *path, char *cmd_path, char **cmd)
{
	char	*tmp;

	free(cmd_path);
	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		px_free_arg(cmd);
		px_free_arg(paths);
		px_error_abort("pipex: ft_strjoin: path + '/' failed", EXIT_FAILURE);
	}
	cmd_path = ft_strjoin(tmp, cmd[0]);
	if (!cmd_path)
	{
		free(tmp);
		px_free_arg(cmd);
		px_free_arg(paths);
		px_error_abort("pipex: ft_strjoin: path + cmd failed", EXIT_FAILURE);
	}
	free(tmp);
	return (cmd_path);
}
