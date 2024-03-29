/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_gen_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamuro <csamuro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 06:20:52 by csamuro           #+#    #+#             */
/*   Updated: 2021/09/02 07:11:30 by csamuro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse	*pars_gen_fill(char **argv, unsigned short oper)
{
	t_parse	*pars;

	pars = (t_parse *)malloc(sizeof(t_parse));
	if (!pars)
		exit ((short)ret_perr("malloc err -> pars") + 1);
	pars->argv = argv;
	pars->oper = oper;
	return (pars);
}
