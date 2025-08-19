/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_init_input_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:06:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/19 19:14:54 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_parse_file_fd(char **argv, int pipefd);
static void	px_parse_heredoc_fd(char **argv, int pipefd, int cmd_count);
static void	px_heredoc_prompt(int pipe_count);

void	px_init_input_fd(t_vars *vars, char **argv)
{
	int	pipefd[2];

	if (px_input_is_heredoc(argv[1]))
	{
		if (pipe(pipefd) == -1)
			px_perror_exit("pipex: pipe");
		px_parse_heredoc_fd(argv, pipefd[1], vars);
		vars->input_fd = pipefd[0];
		close(pipefd[1]);
	}
	else
		px_parse_file_fd(argv, vars);
	return ;
}

static void	px_parse_file_fd(char **argv, t_vars *vars)
{
	vars->input_fd = open(argv[1], O_RDONLY);
	if (vars->input_fd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(argv[1], STDERR_FILENO);
	}
	return ;
}

static void	px_parse_heredoc_fd(char **argv, int pipefd, t_vars *vars)
{
	size_t	limiter_len;
	char	*line;

	limiter_len = ft_strlen(argv[2]);
	px_heredoc_prompt(vars->cmd_count - 1);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strlen(line) == limiter_len + 1
			&& !ft_strncmp(line, argv[2], limiter_len))
			break ;
		write(pipefd, line, ft_strlen(line));
		free(line);
		px_heredoc_prompt(vars->cmd_count - 1);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
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
