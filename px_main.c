/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:35:16 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/29 19:48:25 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_valid_check(int argc, char **argv);
static void px_parse_input(char *input_file, int *pipefd);

int	main(int argc, char **argv)
{
	px_valid_check(argc, argv);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
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
	{
		perror("access");
		exit(EXIT_FAILURE);
	}
	return ;
}

static void px_parse_input(char **argv, int *pipefd)
{
	int		fd;
	char	*line;

	if (ft_strlen(argv[1]) == 8 && !strncmp(argv[1], HERE_DOC, 8))
		fd = STDIN_FILENO;
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
	}
	px_get_input(fd, line, argv, pipefd);
	return ;
}

static void	px_get_input(int fd, char *line, char **argv, int *pipefd)
{
	line = get_next_line(fd);
	while (line)
	{
		if (fd == STDIN_FILENO && ft_strlen(line) == ft_strlen(argv[2]) + 1
	  		&& !strncmp(line, argv[2], ft_strlen(argv[2])))
			break ;
		write(pipefd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(fd);
	}
	if (fd == STDIN_FILENO)
		free(line);
	return ;
}
