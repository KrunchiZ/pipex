/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchiang <kchiang@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:19:41 by kchiang           #+#    #+#             */
/*   Updated: 2025/08/20 01:34:50 by kchiang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	px_count_arg(const char *s, const char *set, int *count);
static int	px_fillhost(char **host, const char *s, const char *set, int count);
static void	px_arglen(const char *s, const char *set, int *len);
static void	px_free_host(char **host, int depth);

char	**px_split(const char *s, const char *set)
{
	int		arg_count;
	char	**host;

	if (!s)
		return (NULL);
	arg_count = 0;
	px_count_arg(s, set, &arg_count);
	host = malloc((arg_count + 1) * sizeof(char *));
	if (!host)
		return (NULL);
	host[arg_count] = NULL;
	if (px_fillhost(host, s, set, arg_count))
		return (host);
	return (NULL);
}

static void	px_count_arg(const char *s, const char *set, int *count)
{
	int			i;
	t_quotes	quotes;

	i = 0;
	while (s[i])
	{
		quotes = (t_quotes){false};
		if (!ft_strchr(set, s[i]))
		{
			(*count)++;
			if (s[i] == '\'')
				quotes.single = true;
			else if (s[i] == '\"')
				quotes.dbl = true;
			i++;
			while ((quotes.single && s[i] && s[i] != '\'')
				|| (quotes.dbl && s[i] && s[i] != '\"'))
				i++;
			while (s[i] && !ft_strchr(set, s[i]))
				i++;
		}
		else
			i++;
	}
	return ;
}

static int	px_fillhost(char **host, const char *s, const char *set, int count)
{
	int	i;
	int	depth;
	int	arglen;

	i = 0;
	depth = 0;
	while (depth < count)
	{
		if (!ft_strchr(set, s[i])
			&& (i == 0 || (i > 0 && ft_strchr(set, s[i - 1]))))
		{
			px_arglen(&s[i], set, &arglen);
			if (s[i] == '\'' || s[i] == '\"')
			{
				i++;
				arglen -= 2;
			}
			host[depth] = malloc(arglen + 1);
			if (host == NULL)
				return (px_free_host(host, depth), false);
			ft_strlcpy(host[depth++], &s[i], arglen + 1);
		}
		i++;
	}
	return (true);
}

static void	px_arglen(const char *s, const char *set, int *len)
{
	t_quotes	quotes;

	*len = 0;
	while (*s && !ft_strchr(set, *s))
	{
		quotes = (t_quotes){false};
		(*len)++;
		if (*s == '\'')
			quotes.single = true;
		else if (*s == '\"')
			quotes.dbl = true;
		s++;
		while ((quotes.single && *s && *s != '\'')
			|| (quotes.dbl && *s && *s != '\"'))
		{
			(*len)++;
			s++;
		}
		while (*s && !ft_strchr(set, *s))
		{
			(*len)++;
			s++;
		}
	}
	return ;
}

static void	px_free_host(char **host, int depth)
{
	while (depth >= 0)
		free(host[depth--]);
	free(host);
	return ;
}
