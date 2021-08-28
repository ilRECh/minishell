/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_sub_argutils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamuro <csamuro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 06:21:27 by csamuro           #+#    #+#             */
/*   Updated: 2021/08/29 02:09:45 by csamuro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	remove_quotation(t_list *lst)
{
	t_list	*tmp;
	int		i;

	i = -1;
	tmp = lst;
	tmp->cur = tmp->head->next;
	while (tmp->cur)
	{
		while (((char *)(tmp->cur->content))[++i])
			if (ft_strchr("\'\"", ((char *)tmp->cur->content)[i]))
				ft_memmove(&tmp->cur->content[i],
					&tmp->cur->content[i + 1],
					ft_strlen(&tmp->cur->content[i]));
		i = -1;
		tmp->cur = tmp->cur->next;
	}
}

t_parse	*to_separate_util_args(t_parse *parse, t_list *lst)
{
	char	*beg;
	int		i;

	i = 0;
	free(*(parse->argv));
	free((parse->argv));
	if (is_builtins(trimmer(((char *)lst->head->content), "\"\'")))
		beg = ft_strdup(trimmer(((char *)lst->head->content), "\"\'"));
	else
		beg = is_util_exists(trimmer(((char *)lst->head->content), "\"\'"));
	if (beg)
	{
		free(lst->head->content);
		lst->head->content = beg;
	}
	else
		ft_lstadd_front(lst, NULL);
	trimmer(lst->head->content, "\"\'");
	// lst_trimmer(lst);
	parse->argv = list_to_char2(lst);
	ft_lstclear(lst, NULL);
	free(lst);
	return (parse);
}

void	sub_repetat(t_list *lst, char **str, int *i)
{
	ft_lstadd_back(lst, ft_strndup(*str, *i));
	(*str) += (*i);
	while (*(*str) && ft_isspace(*(*str)))
		(*str)++;
	(*i) = -1;
}

t_parse	*sub_parse(t_parse *tosub_pars)
{
	t_list	*lst;
	char	*str;
	_Bool	chk;
	int		i;

	i = -1;
	chk = FALSE;
	str = *(tosub_pars->argv);
	lst = (t_list *)ft_calloc(sizeof(t_list), 1);
	while (*str)
	{
		if (ft_strchr("\"\'", str[++i]) && (!i || str[i - 1] != '\\'))
		{
			while (str[i] && !ft_isspace(str[i]))
				if (sub_sub_quote(str, &i))
					break ;
			sub_repetat(lst, &str, &i);
		}
		else if (!str[i] || ft_isspace(str[i]))
			sub_repetat(lst, &str, &i);
	}
	return (to_separate_util_args(tosub_pars, lst));
}

char	*set_last_exit_app(char *s1, char *s2);
char	*set_env(char *line, char *ln);

char	*set_local(char *str, char *base_str)
{
	char	*start;
	char	*end;
	char	*word;
	char	*new_value;

	(void)base_str;
	word = NULL;
	new_value = NULL;
	start = (end = str);
	while (start > base_str && !ft_isspace(*start - 1))
		start--;
	while (*end && !ft_isspace(*end + 1) && end++)
		if (ft_strchr("\'\"", *end) && *(end - 1) != '\\')
			end += skip_quote(end, 0, *end);
	if (start != str && end != str)
	{
		word = ft_strndup(start, str - start);
		new_value = ft_strndup(str + 1, end - str);
		setval_local(word, new_value);
	}
	return (NULL);
}

unsigned int	dollr(t_parse *parse)
{
	unsigned int	i;
	char			*tmp;
	char			*str;
	char			*base_str;

	i = -1;
	tmp = NULL;
	while (parse->argv[++i])
	{
		str = parse->argv[i];
		base_str = parse->argv[i];
		while (*str)
		{
			if (*str == '=')
			{
				set_local(str, base_str);
				return (1);
			}
			if (*str == '$' && (base_str == str || *(str - 1) != '\\'))
			{
				if (*(str + 1) == '?')
					tmp = set_last_exit_app(base_str, str);
				else
					tmp = set_env(base_str, str + 1);
				if (tmp)
				{
					free (base_str);
					str = tmp + (str - base_str);
					base_str = tmp;
				}
			}
			str++;
		}
		parse->argv[i] = base_str;
	}
	return (0);
}

t_list	*split_sub_argutils(t_list *lst)
{
	t_parse	*parse;

	lst->cur = lst->head;
	while (lst->cur)
	{
		parse = (t_parse *)lst->cur->content;
		if ((!parse->argv || !*parse->argv || !**parse->argv) && \
		parse->oper > 0 && parse->oper < 5)
			return ((t_list *)ret_perr("redirection from/to is empty"));
		if (parse->argv && *parse->argv \
			&& !(parse->oper > 4 && parse->oper < 9) \
			&& !ft_strncmp(CASE, *parse->argv, ft_strlen(CASE)))
		{
			if (!split_sub_argutils((\
			(t_list *)((t_parse *)lst->cur->content)->argv[1])))
				return (NULL);
		}
		else if (parse->argv && *parse->argv
			&& !(parse->oper > 4 && parse->oper < 9))
		{
			if (dollr(parse))
			{
				ft_lstdelone(lst, free_parse);
				if (lst->cur && lst->cur->content && !((t_parse *)(lst->cur->content))->argv)
					ft_lstdelone(lst, free_parse);
				continue ;
			}
			lst->cur->content = sub_parse(parse);
		}
		lst->cur = lst->cur->next;
	}
	if (!check_line_lst(lst))
		return (NULL);
	return (lst);
}
