/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:52:19 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/31 13:57:24 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define HERE_DOC	"here_doc"

typedef struct s_vars
{
	t_uchar	append_mode;
	int		cmd_count;
}			t_vars;

void	px_perror_exit(char *str);
void	px_init_input_fd(int *fd, char **argv, t_vars *vars);
void	px_exec_pipex(t_vars vars, char **argv, int input_fd);

#endif
