/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:06:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/19 17:29:21 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_init_input_fd(int *pipefd, char **argv, int cmd_count);
static void	px_parse_heredoc_fd(char **argv, int pipefd, int cmd_count);
static void	px_parse_file_fd(char **argv, int pipefd);
static void	px_heredoc_prompt(int pipe_count);

void	px_exec_first_child(char **argv, t_vars vars)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		px_perror_exit("pipex: pipe");
	vars.pid = fork();
	if (vars.pid == -1)
		px_perror_exit("pipex: fork");
	if (vars.pid == 0)
		px_init_input_fd(pipefd, argv, vars.cmd_count);
	else
	{
		close(pipefd[1]);
		if (vars.append_mode)
			argv++;
		px_exec_child_process(vars, argv + 2, pipefd[0]);
	}
	return ;
}

static void	px_init_input_fd(int *pipefd, char **argv, int cmd_count)
{
	close(pipefd[0]);
	if (px_input_is_heredoc(argv[1]))
		px_parse_heredoc_fd(argv, pipefd[1], cmd_count);
	else
		px_parse_file_fd(argv, pipefd[1]);
	close(pipefd[1]);
	return ;
}

static void	px_parse_heredoc_fd(char **argv, int pipefd, int cmd_count)
{
	size_t	limiter_len;
	char	*line;

	limiter_len = ft_strlen(argv[2]);
	px_heredoc_prompt(cmd_count - 1);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strlen(line) == limiter_len + 1
			&& !ft_strncmp(line, argv[2], limiter_len))
			break ;
		write(pipefd, line, ft_strlen(line));
		free(line);
		px_heredoc_prompt(cmd_count - 1);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	return ;
}

static void	px_parse_file_fd(char **argv, int pipefd)
{
	int		fd;
	char	*line;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		px_error_abort(argv[1]);
	}
	line = get_next_line(fd);
	while (line)
	{
		write(pipefd, line, ft_strlen(line));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return ;
}

static void	px_heredoc_prompt(int pipe_count)
{
	int	i;

	i = 0;
	while (i++ < pipe_count)
		write(STDOUT_FILENO, "pipe ", 5);
	write(STDOUT_FILENO, "here_doc> ", 10);
	return ;
}
