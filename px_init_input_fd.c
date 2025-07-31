/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:31:21 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 19:26:49 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	px_input_is_heredoc(char *input_file);
static void	px_parse_heredoc_fd(char **argv, int pipefd);

void	px_init_input_fd(int *fd, char **argv, t_vars *vars)
{
	int		pipefd[2];

	if (px_input_is_heredoc(argv[1]))
	{
		vars->append_mode = true;
		if (pipe(pipefd) == -1)
			px_perror_exit("pipe");
		px_parse_heredoc_fd(argv, pipefd[1]);
		if (dup2(pipefd[0], *fd) == -1)
			px_perror_exit("dup2");
		(void)(close(pipefd[0]) + close(pipefd[1]));
	}
	else
	{
		*fd = open(argv[1], O_RDONLY);
		if (*fd == -1)
			px_perror_exit("open");
	}
	return ;
}

static int	px_input_is_heredoc(char *input_file)
{
	size_t	heredoc_len;

	heredoc_len = ft_strlen(HERE_DOC);
	if (ft_strlen(input_file) == heredoc_len
		&& !ft_strncmp(input_file, HERE_DOC, heredoc_len))
		return (true);
	return (false);
}

static void	px_parse_heredoc_fd(char **argv, int pipefd)
{
	size_t	limiter_len;
	char	*line;

	limiter_len = ft_strlen(argv[2]);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strlen(line) == limiter_len + 1
			&& !ft_strncmp(line, argv[2], limiter_len))
			break ;
		write(pipefd, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	return ;
}
