/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/23 01:31:21 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_parent_process(t_vars *vars, char ***argv, int *current);
static void	px_child_process(t_vars *vars, char **argv, int current);
static char	**px_init_cmd(char **argv);
static void	px_init_output(t_vars *vars, int current);

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_vars *vars, char **argv)
{
	int	current;

	current = 0;
	while (current < vars->cmd_count)
	{
		if (pipe(vars->pipefd) == -1)
			px_perror_exit("pipex: pipe", EXIT_FAILURE);
		vars->pid = fork();
		if (vars->pid == -1)
			px_perror_exit("pipex: fork", EXIT_FAILURE);
		if (vars->pid == 0)
			px_child_process(vars, argv, current);
		else
			px_parent_process(vars, &argv, &current);
	}
	close(vars->outfd);
	return ;
}

static void	px_parent_process(t_vars *vars, char ***argv, int *current)
{
	close(vars->pipefd[1]);
	if (vars->input_fd == -1)
		vars->input_fd = vars->pipefd[0];
	else
	{
		if (dup2(vars->pipefd[0], vars->input_fd) == -1)
			px_perror_exit("pipex: dup2", EXIT_FAILURE);
		close(vars->pipefd[0]);
	}
	(*current)++;
	(*argv)++;
	if (*current == vars->cmd_count)
		close(vars->input_fd);
	return ;
}

static void	px_child_process(t_vars *vars, char **argv, int current)
{
	char	**cmd;
	char	*execpath;

	if (vars->input_fd == -1)
		px_closefd_exit(vars);
	close(vars->pipefd[0]);
	if (dup2(vars->input_fd, STDIN_FILENO) == -1)
		px_perror_exit("pipex: dup2", EXIT_FAILURE);
	close(vars->input_fd);
	px_init_output(vars, current);
	cmd = px_init_cmd(argv);
	execpath = px_get_path(cmd, vars->envp);
	if (!execpath)
		px_free_exit(cmd, "pipex: ft_strdup failed", EXIT_FAILURE);
	execve(execpath, cmd, vars->envp);
	px_cmd_error(execpath, cmd);
}

static char	**px_init_cmd(char **argv)
{
	char	**cmd;

	cmd = px_split(*argv, WHITESPACE);
	if (!cmd)
		px_error_abort("pipex: px_split failed", EXIT_FAILURE);
	if (!*cmd)
	{
		free(cmd);
		cmd = malloc(2 * sizeof(char *));
		if (!cmd)
			px_error_abort("pipex: malloc failed", EXIT_FAILURE);
		cmd[1] = NULL;
		cmd[0] = ft_strdup(*argv);
		if (!cmd[0])
			px_error_abort("pipex: ft_strdup failed", EXIT_FAILURE);
	}
	return (cmd);
}

static void	px_init_output(t_vars *vars, int current)
{
	if (current == vars->cmd_count - 1)
	{
		close(vars->pipefd[1]);
		if (vars->outfd == -1)
			exit(EXIT_FAILURE);
		if (dup2(vars->outfd, STDOUT_FILENO) == -1)
			px_perror_exit("pipex: dup2", EXIT_FAILURE);
		close(vars->outfd);
	}
	else
	{
		close(vars->outfd);
		if (dup2(vars->pipefd[1], STDOUT_FILENO) == -1)
			px_perror_exit("pipex: dup2", EXIT_FAILURE);
		close(vars->pipefd[1]);
	}
	return ;
}
