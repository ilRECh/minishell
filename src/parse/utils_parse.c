/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamuro <csamuro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 06:21:44 by csamuro           #+#    #+#             */
/*   Updated: 2021/09/03 02:17:59 by csamuro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  0 = no free, 1 = s1 free, 2 = s2 free, 3 = s1 & s2 free;
char	*ft_strjoin_free(char *s1, char *s2, short fr)
{
	char	*res;

	res = NULL;
	if (s1 && s2)
		res = ft_strjoin(s1, s2);
	else if (s1)
		res = ft_strdup(s1);
	else if (s2)
		res = ft_strdup(s2);
	if ((s1 && fr == 1) || (fr == 3 && s1 && s2))
		free(s1);
	if ((s2 && fr == 2) || (fr == 3 && s1 && s2))
		free(s2);
	return (res);
}

_Bool	sub_sub_quote(char *str, int *i)
{
	int		k;
	char	c;

	k = *i;
	c = str[k];
	if ((str[k] == '\"' || str[k] == '\'') && str[k - 1] != '\\')
	{
		if (c == '\"')
			while (str[++k] && (!((str[k] == '\"')
						|| (str[k] == '\"' && str[k - 1] == '\\'))));
		else if (c == '\'')
			while (str[++k] && (!((str[k] == '\''))))
				;
	}
	*i = k + 1;
	if ((!str[k] || !str[k + 1] || ft_isspace(str[k + 2]) \
	|| ft_strchr("\"\'", str[k + 1])) && str[k + 1] != '\\')
		return (TRUE);
	return (FALSE);
}

t_list	*check_line_lst(t_list *lst)
{
	t_dlist	*tmp;

	tmp = lst->head;
	while (tmp)
	{
		if (tmp->content && \
		((t_parse *)(tmp->content))->argv && \
		((t_parse *)(tmp->content))->argv[0])
			return (lst);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*set_last_exit_app(char *s1, char *s2)
{
	char	*tmp;

	tmp = ft_strndup(s1, s2 - s1);
	tmp = ft_strjoin_free(tmp, ft_itoa(g_param->ret), 3);
	tmp = ft_strjoin_free(tmp, s2 + 2, 1);
	return (tmp);
}

char	*get_some_var(char *str)
{
	char	*tmp;

	tmp = getvalue(str);
	if (!tmp)
		tmp = getval_local(str);
	free (str);
	return (tmp);
}
