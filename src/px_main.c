/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/01 11:39:14 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_file_valid_check(int argc, char **argv);

int	main(int argc, char **argv, char **envp)
{
	int		input_fd;
	t_vars	vars;

	px_file_valid_check(argc, argv);
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
	return (EXIT_SUCCESS);
}

static void	px_file_valid_check(int argc, char **argv)
{
	int	i;

	if (ft_strncmp(argv[1], HERE_DOC, 8))
	{
		if (argc < 5)
			px_error_abort("error: Invalid arguments.");
		if (access(argv[1], F_OK) == 0 && access(argv[1], R_OK) == -1)
			px_perror_exit(argv[1]);
	}
	else if (argc < 6)
		px_error_abort("error: Invalid arguments.");
	if (access(argv[argc - 1], F_OK) == 0 && access(argv[argc - 1], W_OK) == -1)
		px_perror_exit(argv[argc - 1]);
	i = 1;
	while (i < argc)
	{
		if (argv[i++][0] == '\0')
			px_error_abort("error: Invalid arguments.");
	}
	return ;
}
