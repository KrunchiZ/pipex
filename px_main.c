/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 13:52:42 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_file_valid_check(int argc, char **argv);

int	main(int argc, char **argv)
{
	int		input_fd;
	t_vars	vars;

	px_file_valid_check(argc, argv);
	vars.append_mode = false;
	px_init_input_fd(&input_fd, argv, &vars);
	vars.cmd_count = argc - 3;
	if (vars.append_mode)
		argv++;
	px_exec_pipex(vars, argv + 2, input_fd);
	return (EXIT_SUCCESS);
}

static void	px_file_valid_check(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_putendl_fd("error: Invalid arguments.", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (!access(argv[1], F_OK) || !access(argv[1], R_OK)
		|| !access(argv[argc - 1], F_OK) || !access(argv[argc - 1], W_OK))
		px_perror_exit("access");
	return ;
}
