/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_exec_first_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:06:33 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/19 21:16:37 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	px_init_input_fd(int *pipefd, char **argv, t_vars vars);

void	px_exec_first_child(char **argv, t_vars vars)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		px_perror_exit("pipex: pipe");
	if (!vars.append_mode && access(argv[1], R_OK) == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		px_error_abort(argv[1]);
	}
	vars.pid = fork();
	if (vars.pid == -1)
		px_perror_exit("pipex: fork");
	if (vars.pid == 0)
		px_init_input_fd(pipefd, argv, vars);
	else
	{
		close(pipefd[1]);
		if (vars.append_mode)
			argv++;
		px_exec_child_process(vars, argv + 2, pipefd[0]);
	}
	return ;
}

static void	px_init_input_fd(int *pipefd, char **argv, t_vars vars)
{
	int		fd;
	char	buffer[100];
	int		read_bytes;

	close(pipefd[0]);
	if (vars.append_mode)
		fd = vars.input_fd;
	else
		fd = open(argv[1], O_RDONLY);
	read_bytes = read(fd, buffer, 100);
	while (read_bytes)
	{
		if (read_bytes == -1)
			px_perror_exit("pipex: read");
		write(pipefd[1], buffer, read_bytes);
		read_bytes = read(fd, buffer, 100);
	}
	close(fd);
	close(pipefd[1]);
	return ;
}
