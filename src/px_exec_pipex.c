/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/01 02:23:18 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_exec_child_process(t_vars vars, char **argv, int *pipefd,
				int input_fd);
static void	px_init_output_fd(t_vars vars, char *file, int *pipefd);

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_vars vars, char **argv, int input_fd)
{
	pid_t	pid;
	int		pipefd[2];

	if (vars.cmd_count < 1)
		return ;
	if (pipe(pipefd) == -1)
		px_perror_exit("pipe");
	pid = fork();
	if (pid == -1)
		px_perror_exit("fork");
	else if (pid == 0)
		px_exec_child_process(vars, argv, pipefd, input_fd);
	else
	{
		close(pipefd[1]);
		close(input_fd);
		vars.cmd_count--;
		px_exec_pipex(vars, argv + 1, pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	return ;
}

static void	px_exec_child_process(t_vars vars, char **argv, int *pipefd
	, int input_fd)
{
	char	**cmd;
	char	*execpath;

	close(pipefd[0]);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		px_perror_exit("dup2");
	close(input_fd);
	px_init_output_fd(vars, argv[1], pipefd);
	cmd = ft_split(*argv, WHITESPACE);
	if (!cmd)
		px_error_abort("error: ft_split failed.");
	if (**cmd == '\0')
		px_error_abort("error: Empty cmd string.");
	execpath = px_get_path(cmd[0], vars.envp);
	execve(execpath, cmd, vars.envp);
	free(execpath);
	px_perror_exit("execve");
	return ;
}

static void	px_init_output_fd(t_vars vars, char *file, int *pipefd)
{
	int		out_fd;

	if (vars.cmd_count > 1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			px_perror_exit("dup2");
		close(pipefd[1]);
	}
	else
	{
		close(pipefd[1]);
		if (vars.append_mode == true)
			out_fd = open(file, O_WRONLY | O_CREAT | O_APPEND);
		else
			out_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC);
		if (out_fd == -1)
			px_perror_exit(file);
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			px_perror_exit("dup2");
		close(out_fd);
	}
	return ;
}
