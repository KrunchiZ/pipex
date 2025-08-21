/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_is_absolute_path.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:37:25 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/21 14:42:07 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	px_is_absolute_path(char *cmd_path)
{
	return (*cmd_path == '/' || !ft_strncmp(cmd_path, "./", 2)
		|| !ft_strncmp(cmd_path, "~/", 2));
}
