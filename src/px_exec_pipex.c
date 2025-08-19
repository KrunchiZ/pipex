/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 01:14:41 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_parent_process(t_vars *vars, char ***argv, int *current_cmd);
static void	px_child_process(t_vars vars, char **argv, int current_cmd);
static void	px_init_output(t_vars vars, int current_cmd);
static void	px_dup_filefd(t_vars vars);

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_vars vars, char **argv)
{
	int	current_cmd;
	int	i;

	current_cmd = 1;
	while (current_cmd <= vars.cmd_count)
	{
		if (pipe(vars.pipefd) == -1)
			px_perror_exit("pipex: pipe");
		vars.pid = fork();
		if (vars.pid == -1)
			px_perror_exit("pipex: fork");
		if (vars.pid == 0)
			px_child_process(vars, argv, current_cmd);
		else
			px_parent_process(&vars, &argv, &current_cmd);
	}
	i = 0;
	while (i++ < vars.cmd_count)
		wait(NULL);
	return ;
}

static void	px_parent_process(t_vars *vars, char ***argv, int *current_cmd)
{
	close(vars->pipefd[1]);
	if (vars->input_fd == -1)
		vars->input_fd = vars->pipefd[0];
	else
	{
		if (dup2(vars->pipefd[0], vars->input_fd) == -1)
			px_perror_exit("pipex: dup2");
		close(vars->pipefd[0]);
	}
	(*current_cmd)++;
	(*argv)++;
	return ;
}

static void	px_child_process(t_vars vars, char **argv, int current_cmd)
{
	char	**cmd;
	char	*execpath;

	close(vars.pipefd[0]);
	if (vars.input_fd == -1)
		exit(EXIT_FAILURE);
	if (dup2(vars.input_fd, STDIN_FILENO) == -1)
		px_perror_exit("pipex: dup2");
	close(vars.input_fd);
	px_init_output(vars, current_cmd);
	cmd = px_split(*argv, WHITESPACE);
	if (!cmd)
		px_error_abort("pipex: px_split failed");
	execpath = px_get_path(cmd, vars.envp);
	if (!execpath)
		px_error_abort("pipex: ft_strdup failed");
	execve(execpath, cmd, vars.envp);
	free(execpath);
	ft_putstr_fd("pipex: Command not found: ", STDERR_FILENO);
	ft_putendl_fd(cmd[0], STDERR_FILENO);
	px_free_arg(cmd);
	exit(EXIT_FAILURE);
	return ;
}

static void	px_init_output(t_vars vars, int current_cmd)
{
	if (current_cmd == vars.cmd_count)
		px_dup_filefd(vars);
	else
	{
		if (dup2(vars.pipefd[1], STDOUT_FILENO) == -1)
			px_perror_exit("pipex: dup2");
		close(vars.pipefd[1]);
	}
	return ;
}

static void	px_dup_filefd(t_vars vars)
{
	close(vars.pipefd[1]);
	if (vars.outfd == -1)
		exit(EXIT_FAILURE);
	if (dup2(vars.outfd, STDOUT_FILENO) == -1)
		px_perror_exit("pipex: dup2");
	close(vars.outfd);
	return ;
}
