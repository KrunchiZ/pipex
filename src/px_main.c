/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/15 12:43:59 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_arg_check(int argc, char **argv, t_vars *vars);
static void	px_open_outfd(t_vars *vars, char *file);
static void	px_init_input_fd(int *input_fd, char **argv);
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
		px_exec_child_process(vars, argv + 2, input_fd);
	}
	else
	{
		close(vars.pipefd[1]);
		vars.cmd_count--;
		waitpid(vars.pid, NULL, 0);
		px_exec_pipex(vars, argv + 3, vars.pipefd[0]);
	}
	return (EXIT_SUCCESS);
}

static void	px_arg_check(int argc, char **argv, t_vars *vars)
{
	if (argc != 5)
		px_error_abort("pipex: Invalid arguments.");
	vars->cmd_count = argc - 3;
	px_open_outfd(vars, argv[argc - 1]);
	return ;
}

static void	px_open_outfd(t_vars *vars, char *file)
{
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

static void	px_init_input_fd(int *input_fd, char **argv)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		px_perror_exit("pipex: pipe");
	*input_fd = pipefd[0];
	px_parse_file_fd(argv, pipefd[1]);
	close(pipefd[1]);
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
