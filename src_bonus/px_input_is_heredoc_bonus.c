/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_input_is_heredoc_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 20:01:23 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/05 22:15:48 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	px_input_is_heredoc(char *input_file)
{
	size_t	heredoc_len;

	heredoc_len = ft_strlen(HERE_DOC);
	if (ft_strlen(input_file) == heredoc_len
		&& !ft_strncmp(input_file, HERE_DOC, heredoc_len))
		return (true);
	return (false);
}
