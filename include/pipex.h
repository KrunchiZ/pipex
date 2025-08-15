/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:52:19 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/15 12:29:07 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

# define CHMOD_666	0666

/* Whitespaces delimiter for ft_split.
 * */
# define WHITESPACE	" \t\n\v\f\r"

/* Struct for px_split.
 * */
typedef struct s_quotes
{
	int	single;
	int	dbl;
}		t_quotes;

typedef struct s_vars
{
	char	**envp;
	int		cmd_count;
	int		pipefd[2];
	int		outfd;
	pid_t	pid;
}			t_vars;

void	px_perror_exit(char *str);
void	px_error_abort(char *str);
void	px_free_arg(char **arg);
void	px_exec_child_process(t_vars vars, char **argv, int input_fd);
void	px_exec_pipex(t_vars vars, char **argv, int input_fd);
char	**px_split(const char *s, const char *set);
char	*px_get_path(char **cmd, char **envp);

#endif
