/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:52:19 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/22 14:49:10 by kchiang          ###   ########.fr       */
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

# define HERE_DOC		"here_doc"
# define CHMOD_666		0666
# define PERM_DENIED	126
# define NOT_FOUND		127

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
	char	*infile;
	t_uchar	append_mode;
	int		cmd_count;
	int		pipefd[2];
	int		outfd;
	int		input_fd;
	pid_t	pid;
}			t_vars;

void	px_closefd_exit(t_vars *vars);
void	px_perror_exit(char *str, int exit_code);
void	px_error_abort(char *str, int exit_code);
void	px_free_arg(char **arg);
void	px_free_exit(char **cmd, char *str, int exit_code);
int		px_input_is_heredoc(char *input_file);
void	px_parse_infile_fd(t_vars *vars);
void	px_parse_heredoc_fd(char **argv, t_vars *vars);
void	px_exec_pipex(t_vars *vars, char **argv);
char	**px_split(const char *s, const char *set);
char	*px_get_path(char **cmd, char **envp);
void	px_cmd_error(char *execpath, char **cmd);

#endif
