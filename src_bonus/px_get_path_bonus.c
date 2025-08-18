/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/18 10:57:30 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	**px_parse_path_env(char **envp, char **cmd, char *cmd_path);
static char	*px_get_next_path(char **paths, char *cmd_path, char **cmd);
static char	*px_join_path(char **paths, char *path, char *cmd_path, char **cmd);

char	*px_get_path(char **cmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;

	if (cmd[0] && access(cmd[0], X_OK) == 0)
		return (ft_strdup(cmd[0]));
	cmd_path = ft_strjoin("./", cmd[0]);
	if (!cmd_path)
	{
		px_free_arg(cmd);
		px_error_abort("pipex: ft_strjoin: './' + cmd failed");
	}
	if (access(cmd_path, X_OK) == 0)
		return (cmd_path);
	env_paths = px_parse_path_env(envp, cmd, cmd_path);
	return (px_get_next_path(env_paths, cmd_path, cmd));
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
		px_error_abort("pipex: ft_split: malloc failed");
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
		if (access(cmd_path, X_OK) == 0)
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
		px_error_abort("pipex: ft_strjoin: path + '/' failed");
	}
	cmd_path = ft_strjoin(tmp, cmd[0]);
	if (!cmd_path)
	{
		free(tmp);
		px_free_arg(cmd);
		px_free_arg(paths);
		px_error_abort("pipex: ft_strjoin: path + cmd failed");
	}
	free(tmp);
	return (cmd_path);
}
