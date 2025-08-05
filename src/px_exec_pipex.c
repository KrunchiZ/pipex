/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/05 15:43:58 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_exec_child_process(t_vars vars, char **argv, int input_fd);
static void	px_init_output(t_vars vars, char *file, char *execpath, char **cmd);
static void	px_dup_filefd(t_vars vars, char *file, char *execpath, char **cmd);

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_vars vars, char **argv, int input_fd)
{
	pid_t	pid;

	if (vars.cmd_count < 1)
		return ;
	if (pipe(vars.pipefd) == -1)
		px_perror_exit("pipe");
	pid = fork();
	if (pid == -1)
		px_perror_exit("fork");
	else if (pid == 0)
		px_exec_child_process(vars, argv, input_fd);
	else
	{
		close(vars.pipefd[1]);
		close(input_fd);
		vars.cmd_count--;
		px_exec_pipex(vars, argv + 1, vars.pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	return ;
}

static void	px_exec_child_process(t_vars vars, char **argv, int input_fd)
{
	char	**cmd;
	char	*execpath;

	close(vars.pipefd[0]);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		px_perror_exit("dup2");
	close(input_fd);
	cmd = px_split(*argv, WHITESPACE);
	if (!cmd)
		px_error_abort("error: ft_split failed.");
	execpath = px_get_path(cmd, vars.envp);
	if (!execpath)
		px_error_abort("error: Command not found / Invalid command.");
	px_init_output(vars, argv[1], execpath, cmd);
	execve(execpath, cmd, vars.envp);
	free(execpath);
	px_perror_exit("execve");
	return ;
}

static void	px_init_output(t_vars vars, char *file, char *execpath, char **cmd)
{
	if (vars.cmd_count > 1)
	{
		if (dup2(vars.pipefd[1], STDOUT_FILENO) == -1)
		{
			free(execpath);
			px_free_arg(cmd);
			px_perror_exit("dup2");
		}
		close(vars.pipefd[1]);
	}
	else
		px_dup_filefd(vars, file, execpath, cmd);
	return ;
}

static void	px_dup_filefd(t_vars vars, char *file, char *execpath, char **cmd)
{
	int	outfd;

	close(vars.pipefd[1]);
	if (vars.append_mode == true)
		outfd = open(file, O_WRONLY | O_CREAT | O_APPEND, CHMOD_666);
	else
		outfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, CHMOD_666);
	if (outfd == -1)
	{
		free(execpath);
		px_free_arg(cmd);
		px_perror_exit(file);
	}
	if (dup2(outfd, STDOUT_FILENO) == -1)
	{
		free(execpath);
		px_free_arg(cmd);
		px_perror_exit("dup2");
	}
	close(outfd);
	return ;
}
