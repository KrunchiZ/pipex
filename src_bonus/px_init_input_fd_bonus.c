/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 00:31:21 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/05 22:24:45 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_parse_heredoc_fd(char **argv, int pipefd);

void	px_init_input_fd(int *fd, char **argv, t_vars *vars)
{
	if (px_input_is_heredoc(argv[1]))
	{
		if (pipe(vars->pipefd) == -1)
			px_perror_exit("pipex: pipe");
		vars->append_mode = true;
		px_parse_heredoc_fd(argv, vars->pipefd[1]);
		close(vars->pipefd[1]);
		*fd = vars->pipefd[0];
	}
	else
	{
		*fd = open(argv[1], O_RDONLY);
		if (*fd == -1)
		{
			ft_putstr_fd("pipex: no such file or directory: ", STDERR_FILENO);
			ft_putendl_fd(argv[1], STDERR_FILENO);
		}
	}
	return ;
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
