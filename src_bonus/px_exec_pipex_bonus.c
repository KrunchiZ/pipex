/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/11 18:04:41 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_init_output(t_vars vars, char *file);
static void	px_dup_filefd(t_vars vars, char *file);

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_vars vars, char **argv, int input_fd)
{
	if (vars.cmd_count < 1)
		return ;
	if (pipe(vars.pipefd) == -1)
		px_perror_exit("pipex: pipe");
	vars.pid = fork();
	if (vars.pid == -1)
		px_perror_exit("pipex: fork");
	else if (vars.pid == 0)
		px_exec_child_process(vars, argv, input_fd);
	else
	{
		if (input_fd != -1)
			close(input_fd);
		close(vars.pipefd[1]);
		vars.cmd_count--;
		px_exec_pipex(vars, argv + 1, vars.pipefd[0]);
		waitpid(vars.pid, NULL, 0);
	}
	return ;
}

void	px_exec_child_process(t_vars vars, char **argv, int input_fd)
{
	char	**cmd;
	char	*execpath;

	close(vars.pipefd[0]);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		px_perror_exit("pipex: dup2");
	close(input_fd);
	px_init_output(vars, argv[1]);
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

static void	px_init_output(t_vars vars, char *file)
{
	if (vars.cmd_count <= 1)
		px_dup_filefd(vars, file);
	else
	{
		if (dup2(vars.pipefd[1], STDOUT_FILENO) == -1)
			px_perror_exit("pipex: dup2");
		close(vars.pipefd[1]);
	}
	return ;
}

static void	px_dup_filefd(t_vars vars, char *file)
{
	int	outfd;

	close(vars.pipefd[1]);
	if (vars.append_mode == true)
		outfd = open(file, O_WRONLY | O_CREAT | O_APPEND, CHMOD_666);
	else
		outfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, CHMOD_666);
	if (outfd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		px_error_abort(file);
	}
	if (dup2(outfd, STDOUT_FILENO) == -1)
		px_perror_exit("pipex: dup2");
	close(outfd);
	return ;
}
