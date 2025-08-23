/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/23 22:00:05 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#define IS_FIRST_CHECK true
#define NOT_FIRST_CHECK false

static int	px_path_is_valid(char *cmd_path, int is_first_check);
static char	**px_parse_path_env(char **envp, char **cmd);
static char	*px_join_path(char **env_paths, char *path, char **cmd);

char	*px_get_path(char **cmd, char **envp)
{
	char	**env_paths;
	char	*cmd_path;
	char	**cursor;

	if (cmd[0] && px_path_is_valid(cmd[0], IS_FIRST_CHECK))
		return (ft_strdup(cmd[0]));
	env_paths = px_parse_path_env(envp, cmd);
	if (!env_paths)
		return (ft_strdup(cmd[0]));
	cmd_path = NULL;
	cursor = env_paths;
	while (*cursor)
	{
		free(cmd_path);
		cmd_path = px_join_path(env_paths, *cursor++, cmd);
		if (px_path_is_valid(cmd_path, NOT_FIRST_CHECK))
			break ;
	}
	px_free_arg(env_paths);
	return (cmd_path);
}

static int	px_path_is_valid(char *cmd_path, int is_first_check)
{
	if (access(cmd_path, F_OK) == 0
		|| (is_first_check && ft_strchr(cmd_path, '/')))
		return (true);
	return (false);
}

static char	**px_parse_path_env(char **envp, char **cmd)
{
	char	*env_var;
	char	**env_paths;

	env_var = NULL;
	while (*envp)
	{
		env_var = *envp++;
		if (!ft_strncmp(env_var, "PATH=", 5))
			break ;
	}
	if (!env_var)
		return (NULL);
	env_paths = ft_split(env_var, "=:");
	if (!env_paths)
	{
		px_free_arg(cmd);
		px_error_abort("pipex: ft_split: malloc failed", EXIT_FAILURE);
	}
	return (env_paths);
}

static char	*px_join_path(char **env_paths, char *path, char **cmd)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		px_free_arg(cmd);
		px_free_arg(env_paths);
		px_error_abort("pipex: ft_strjoin: path + '/' failed", EXIT_FAILURE);
	}
	cmd_path = ft_strjoin(tmp, cmd[0]);
	if (!cmd_path)
	{
		free(tmp);
		px_free_arg(cmd);
		px_free_arg(env_paths);
		px_error_abort("pipex: ft_strjoin: path + cmd failed", EXIT_FAILURE);
	}
	free(tmp);
	return (cmd_path);
}
