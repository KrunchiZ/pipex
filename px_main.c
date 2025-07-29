/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/30 01:18:39 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_valid_check(int argc, char **argv);

int	main(int argc, char **argv)
{
	int		input_fd;
	int		cmd_count;
	t_uchar	output_flag;

	px_valid_check(argc, argv);
	if (pipe(pipefd) == -1)
		px_perror_exit("pipe");
	output_flag = 0;
	px_init_input_fd(&input_fd, argv, &output_flag);
	cmd_count = argc - 3;
	px_exec_pipex(cmd_count, );
	return (EXIT_SUCCESS);
}

static void	px_valid_check(int argc, char **argv)
{
	if (argc != 5)
	{
		ft_putendl_fd("Error: Invalid arguments.", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (!access(argv[1], F_OK) || !access(argv[1], R_OK)
		|| !access(argv[argc - 1], F_OK) || !access(argv[argc - 1], W_OK))
		px_perror_exit("access");
	return ;
}
