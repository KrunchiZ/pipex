/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_get_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:24:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 18:58:44 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**parse_path_env(void);

char	*px_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*cmd_path;
	char	*tmp;

	cmd_path = ft_strjoin("./", cmd);
	if (!cmd_path)
		px_error_abort("error: ft_strjoin failed.");
	if (access(cmd_path, X_OK) == 0)
		return (cmd_path);
	paths = px_parse_path_env(envp);
	while (*paths)
	{
		tmp = ft_strjoin(*paths++, "/");
		if (!tmp)
			px_error_abort("error: ft_strjoin failed.");
		free(cmd_path);
		cmd_path = ft_strjoin(tmp, cmd);
		if (!cmd_path)
			px_error_abort("error: ft_strjoin failed.");
		free(tmp);
		if (access(cmd_path, X_OK) == 0)
			break ;
	}
	return (cmd_path);
}

static char	**parse_path_env(char **envp)
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
	free(paths[0]);
	paths++;
	return (paths);
}
