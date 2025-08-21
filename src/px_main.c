/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 20:25:37 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_init_filefd(int argc, char **argv, t_vars *vars);
static void	px_open_outfile(t_vars *vars, char *file);
static void	px_wait_child(t_vars *vars, int *last_status);

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;
	int		last_status;

	vars.envp = envp;
	vars.infile = argv[1];
	vars.cmd_count = argc - 3;
	px_init_filefd(argc, argv, &vars);
	if (vars.append_mode)
		px_exec_pipex(&vars, argv + 3);
	else
		px_exec_pipex(&vars, argv + 2);
	px_wait_child(&vars, &last_status);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	return (EXIT_FAILURE);
}

static void	px_init_filefd(int argc, char **argv, t_vars *vars)
{
	if (px_input_is_heredoc(vars->infile))
	{
		vars->append_mode = true;
		if (argc < 6)
			px_error_abort("pipex: Invalid arguments", EXIT_FAILURE);
		vars->cmd_count--;
		px_parse_heredoc_fd(argv, vars);
	}
	else
	{
		vars->append_mode = false;
		if (argc < 5)
			px_error_abort("pipex: Invalid arguments", EXIT_FAILURE);
		px_parse_infile_fd(vars);
	}
	px_open_outfile(vars, argv[argc - 1]);
	return ;
}

static void	px_open_outfile(t_vars *vars, char *file)
{
	if (vars->append_mode)
		vars->outfd = open(file, O_WRONLY | O_CREAT | O_APPEND, CHMOD_666);
	else
		vars->outfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, CHMOD_666);
	if (vars->outfd == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	}
	return ;
}

static void	px_wait_child(t_vars *vars, int *last_status)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	while (i++ < vars->cmd_count)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == vars->pid)
			*last_status = status;
	}
	return ;
}
