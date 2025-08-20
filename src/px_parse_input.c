/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:06:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 12:10:24 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_heredoc_prompt(int pipe_count);

void	px_parse_infile_fd(t_vars *vars)
{
	vars->input_fd = open(vars->infile, O_RDONLY);
	if (vars->input_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(vars->infile, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
	return ;
}

void	px_parse_heredoc_fd(char **argv, t_vars *vars)
{
	size_t	limiter_len;
	char	*line;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		perror("pipex: pipe");
	limiter_len = ft_strlen(argv[2]);
	px_heredoc_prompt(vars->cmd_count - 1);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strlen(line) == limiter_len + 1
			&& !ft_strncmp(line, argv[2], limiter_len))
			break ;
		write(pipefd[1], line, ft_strlen(line));
		free(line);
		px_heredoc_prompt(vars->cmd_count - 1);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	vars->input_fd = pipefd[0];
	close(pipefd[1]);
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
