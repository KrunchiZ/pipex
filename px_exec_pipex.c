/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_pipex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:29:49 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 13:07:55 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_exec_child_process(t_uchar append_mode, char **argv,
				int *pipefd, int input_fd);

/* External Variable accessing the Environment config in the terminal.
 * Similar to envp from int main(int argc, char **argv, char **envp){}.
 * */
extern char	**environ;

/* Piping function that recurses itself.
 * */
void	px_exec_pipex(t_uchar append_mode, char **argv, int cmd_count
	, int input_fd)
{
	pid_t	pid;
	int		pipefd[2];

	if (cmd_count < 1)
		return ;
	if (pipe(pipefd) == -1)
		px_perror_exit("pipe");
	pid = fork();
	if (pid == -1)
		px_perror_exit("fork");
	else if (pid == 0)
		px_exec_child_process(append_mode, argv, pipefd, input_fd);
	else
	{
		close(pipefd[1]);
		close(input_fd);
		px_exec_pipex(append_mode, argv + 1, cmd_count - 1, pipefd[0]);
		waitpid(pid, NULL, 0);
	}
	return ;
}

static void	px_exec_child_process(t_uchar append_mode, char **argv
	, int *pipefd, int input_fd)
{
	int		out_fd;
	char	**cmd;

	if (dup2(input_f d, STDIN_FILENO) == -1)
		px_perror_exit("dup2");
	close(input_fd) + close(pipefd[0]);
	if (cmd_count > 1)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			px_error_exit("dup2");
		close(pipefd[1]);
	}
	else
	{
		if (append_mode == true)
			out_fd = open(argv[1], O_WRONLY, O_CREAT, O_APPEND);
		else
			out_fd = open(argv[1], O_WRONLY, O_CREAT, O_TRUNC);
		if (out_fd == -1)
			px_perror_exit("open");
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			px_perror_exit("dup2");
		close(out_fd) + close(pipefd[1]);
	}
	/* ft_split argv[0] before parsing to execve.
	 * split[0] for first arg, whole split for second arg.
	 * char *envp[1], initialized to NULL.
	 * */
}
