/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/05 22:14:05 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_arg_check(int argc, char **argv);

int	main(int argc, char **argv, char **envp)
{
	int		input_fd;
	t_vars	vars;
	pid_t	pid;

	px_arg_check(argc, argv);
	if (pipe(vars.pipefd) == -1)
		px_perror_exit("pipex: pipe");
	pid = fork();
	if (pid == 0)
	{
		vars.envp = envp;
		vars.append_mode = false;
		px_init_input_fd(&input_fd, argv, &vars);
		vars.cmd_count = argc - 3;
		if (vars.append_mode)
		{
			argv++;
			vars.cmd_count--;
		}
		px_exec_pipex(vars, argv + 2, input_fd);
	}
	else
		waitpid(pid, NULL, 0);
	return (EXIT_SUCCESS);
}

static void	px_arg_check(int argc, char **argv)
{
	if (!px_input_is_heredoc(argv[1]))
	{
		if (argc < 5)
			px_error_abort("pipex: Invalid arguments.");
	}
	else if (argc < 6)
		px_error_abort("pipex: Invalid arguments.");
	return ;
}
