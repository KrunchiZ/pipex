/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/02 14:28:52 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**px_parse_path_env(char **envp);
static char	*px_get_next_path(char **env, char *cmd_path, char *cmd);
static char	*px_join_path(char **env, char *paths, char *cmd_path, char *cmd);

char	*px_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;

	cmd_path = ft_strjoin("./", cmd);
	if (!cmd_path)
		px_error_abort("error: ft_strjoin failed.");
	if (access(cmd_path, X_OK) == 0)
		return (cmd_path);
	paths = px_parse_path_env(envp);
	return (px_get_next_path(paths, cmd_path, cmd));
}

static char	**px_parse_path_env(char **envp)
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
		px_error_abort("error: ft_split failed.");
	return (paths);
}

static char	*px_get_next_path(char **env, char *cmd_path, char *cmd)
{
	char	**paths;

	paths = env;
	while (*paths)
	{
		cmd_path = px_join_path(env, *paths++, cmd_path, cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			px_free_env(env);
			return (cmd_path);
		}
	}
	px_free_env(env);
	free(cmd_path);
	return (NULL);
}

static char	*px_join_path(char **env, char *paths, char *cmd_path, char *cmd)
{
	char	*tmp;

	free(cmd_path);
	tmp = ft_strjoin(paths, "/");
	if (!tmp)
	{
		px_free_env(env);
		px_error_abort("error: ft_strjoin failed.");
	}
	cmd_path = ft_strjoin(tmp, cmd);
	if (!cmd_path)
	{
		free(tmp);
		px_free_env(env);
		px_error_abort("error: ft_strjoin failed.");
	}
	free(tmp);
	return (cmd_path);
}
