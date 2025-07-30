/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:52:19 by kchiang           #+#    #+#             */
/*   Updated: 2025/07/30 17:36:42 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define HERE_DOC	"here_doc"

void	px_perror_exit(char *str);
void	px_init_input_fd(int *fd, char **argv, t_uchar *output_flag);

#endif
