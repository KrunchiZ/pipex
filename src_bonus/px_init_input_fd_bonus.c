/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:06:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/06 17:37:43 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_parse_heredoc_fd(char **argv, int pipefd, int cmd_count);
static void	px_parse_file_fd(char **argv, int pipefd);
static void	px_heredoc_prompt(int pipe_count);

void	px_init_input_fd(int *input_fd, char **argv, int cmd_count)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		px_perror_exit("pipex: pipe");
	*input_fd = pipefd[0];
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
		ft_putstr_fd("pipex: no such file or directory: ", STDERR_FILENO);
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
