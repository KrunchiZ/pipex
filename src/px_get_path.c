/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/02 21:49:34 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**px_parse_path_env(char **envp, char **cmd, char *cmd_path);
static char	*px_get_next_path(char **env, char *cmd_path, char **cmd);
static char	*px_join_path(char **paths, char *path, char *cmd_path, char **cmd);

char	*px_get_path(char **cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;

	cmd_path = ft_strjoin("./", cmd[0]);
	if (!cmd_path)
	{
		px_free_arg(cmd);
		px_error_abort("error: ft_strjoin failed.");
	}
	if (access(cmd_path, X_OK) == 0)
		return (cmd_path);
	paths = px_parse_path_env(envp, cmd, cmd_path);
	return (px_get_next_path(paths, cmd_path, cmd));
}

static char	**px_parse_path_env(char **envp, char **cmd, char *cmd_path)
{
	char	*env_var;
	char	**paths;

	while (*envp)
	{
		env_var = *envp++;
		if (!ft_strncmp(env_var, "PATH=", 5))
			break ;
	}
	paths = ft_split(env_var, "=:");
	if (!paths)
	{
		free(cmd_path);
		px_free_arg(cmd);
		px_error_abort("error: ft_split failed.");
	}
	return (paths);
}

static char	*px_get_next_path(char **paths, char *cmd_path, char **cmd)
{
	char	**cursor;

	cursor = paths;
	while (*cursor)
	{
		cmd_path = px_join_path(paths, *cursor++, cmd_path, cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			px_free_arg(paths);
			return (cmd_path);
		}
	}
	px_free_arg(cmd);
	px_free_arg(paths);
	free(cmd_path);
	return (NULL);
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
		px_error_abort("error: ft_strjoin path + '/' failed.");
	}
	cmd_path = ft_strjoin(tmp, cmd[0]);
	if (!cmd_path)
	{
		free(tmp);
		px_free_arg(cmd);
		px_free_arg(paths);
		px_error_abort("error: ft_strjoin path + cmd failed.");
	}
	free(tmp);
	return (cmd_path);
}
