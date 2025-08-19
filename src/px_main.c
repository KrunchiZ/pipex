/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/19 21:26:45 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_arg_check(int argc, char **argv, t_vars *vars);
static void	px_parse_heredoc_fd(char **argv, t_vars *vars);
static void	px_heredoc_prompt(int pipe_count);
static void	px_open_outfd(t_vars *vars, char *file);

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;

	vars.envp = envp;
	px_arg_check(argc, argv, &vars);
	if (pipe(vars.pipefd) == -1)
		px_perror_exit("pipex: pipe");
	vars.pid = fork();
	if (vars.pid == -1)
		px_perror_exit("pipex: fork");
	if (vars.pid == 0)
		px_exec_first_child(argv, vars);
	else
	{
		if (vars.append_mode)
			argv++;
		close(vars.pipefd[1]);
		vars.cmd_count--;
		px_exec_pipex(vars, argv + 3, vars.pipefd[0]);
	}
	return (EXIT_SUCCESS);
}

static void	px_arg_check(int argc, char **argv, t_vars *vars)
{
	if (!px_input_is_heredoc(argv[1]))
	{
		vars->append_mode = false;
		if (argc < 5)
			px_error_abort("pipex: Invalid arguments.");
	}
	else
	{
		vars->append_mode = true;
		if (argc < 6)
			px_error_abort("pipex: Invalid arguments.");
	}
	vars->cmd_count = argc - 3;
	if (vars->append_mode)
	{
		vars->cmd_count--;
		px_parse_heredoc_fd(argv, vars);
	}
	px_open_outfd(vars, argv[argc - 1]);
	return ;
}

static void	px_parse_heredoc_fd(char **argv, t_vars *vars)
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

static void	px_open_outfd(t_vars *vars, char *file)
{
	if (vars->append_mode)
		vars->outfd = open(file, O_WRONLY | O_CREAT | O_APPEND, CHMOD_666);
	else
		vars->outfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, CHMOD_666);
	if (vars->outfd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(file, STDERR_FILENO);
	}
	return ;
}
