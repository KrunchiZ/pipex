/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:31:21 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/30 01:19:19 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_parse_heredoc_fd(int fd, char *line, char **argv);

void	px_init_input_fd(int *fd, char **argv, t_uchar output_flag)
{
	char	*line;
	int		pipefd[2];

	if (ft_strlen(argv[1]) == 8 && !ft_strncmp(argv[1], HERE_DOC, 8))
	{
		output_flag |= APPEND;
		if (pipe(pipefd) == -1)
			px_perror_exit("pipe");
		close(pipefd[0]);
		px_parse_heredoc_fd(STDIN_FILENO, line, argv, pipefd[1]);
		if (dup2(pipefd[1], *fd) == -1)
			px_perror_exit("dup2");
		close(pipefd[1]);
	}
	else
	{
		output_FLAG |= OVERWRITE;
		*fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			px_perror_exit("open");
	}
	return ;
}

static void	px_parse_heredoc_fd(int fd, char *line, char **argv, int pipefd)
{
	size_t	limiter_len;

	limiter_len = ft_strlen(argv[2]);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strlen(line) == limiter_len + 1
			&& !ft_strncmp(line, argv[2], limiter_len))
			break ;
		write(pipefd, line, ft_strlen(line));
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	return ;
}
