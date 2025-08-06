/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/06 17:13:41 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_arg_check(int argc, char **argv, t_vars *vars);

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
