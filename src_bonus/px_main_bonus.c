/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/06 00:48:44 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_arg_check(int argc, char **argv, t_vars *vars);
static void	px_init_input_fd(int *input_fd, char **argv);
static void	px_parse_heredoc_fd(char **argv, int pipefd);
static void	px_parse_file_fd(char **argv, int pipefd);

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;
	int		input_fd;

	vars.envp = envp;
	px_arg_check(argc, argv, &vars);
	if (pipe(vars.pipefd) == -1)
		px_perror_exit("pipex: pipe");
	vars.pid = fork();
	if (vars.pid == 0)
	{
		px_init_input_fd(&input_fd, argv);
		if (vars.append_mode)
			argv++;
		px_exec_child_process(vars, argv + 2, input_fd);
	}
	else
	{
		if (vars.append_mode)
			argv++;
		close(vars.pipefd[1]);
		vars.cmd_count--;
		waitpid(vars.pid, NULL, 0);
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
		vars->cmd_count--;
	return ;
}

static void	px_init_input_fd(int *input_fd, char **argv)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		px_perror_exit("pipex: pipe");
	*input_fd = pipefd[0];
	if (px_input_is_heredoc(argv[1]))
		px_parse_heredoc_fd(argv, pipefd[1]);
	else
		px_parse_file_fd(argv, pipefd[1]);
	close(pipefd[1]);
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
