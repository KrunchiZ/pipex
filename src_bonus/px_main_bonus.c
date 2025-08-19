/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/19 16:30:42 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_arg_check(int argc, char **argv, t_vars *vars);
static void	px_open_outfd(t_vars *vars, char *file);

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
		px_init_input_fd(&input_fd, argv, vars.cmd_count);
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
		px_exec_pipex(vars, argv + 3, vars.pipefd[0]);
		waitpid(vars.pid, NULL, 0);
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
	px_open_outfd(vars, argv[argc - 1]);
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
