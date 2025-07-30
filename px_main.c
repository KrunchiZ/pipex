/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/30 20:00:44 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_file_valid_check(int argc, char **argv);

int	main(int argc, char **argv)
{
	int		input_fd;
	int		cmd_count;
	t_uchar	append_mode;

	px_file_valid_check(argc, argv);
	append_mode = false;
	px_init_input_fd(&input_fd, argv, &append_mode);
	cmd_count = argc - 3;
	if (append_mode)
		argv++;
	px_exec_pipex(append_mode, argv + 2, cmd_count, input_fd);
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
