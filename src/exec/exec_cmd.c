/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csamuro <csamuro@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/24 21:22:41 by vcobbler          #+#    #+#             */
/*   Updated: 2021/09/04 01:57:17 by csamuro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child(char **args, t_rdrct *rdrct)
{
	signal(EOF, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_lstclear(&rdrct->in, NULL);
	ft_lstclear(&rdrct->out, NULL);
	if (rdrct->copy.is0)
		close(rdrct->copy.fd[0]);
	if (rdrct->copy.is1)
		close(rdrct->copy.fd[1]);
	if (rdrct->inall.is)
		close(rdrct->inall.pipefd[1]);
	close(rdrct->outall.pipefd[0]);
	if (rdrct->inall.is)
		dup2(rdrct->inall.pipefd[0], 0);
	dup2(rdrct->outall.pipefd[1], 1);
	execve(args[0], args, g_param->env);
	dup2(g_param->stdout_copy, 1);
	printf("no right to execute\n");
	exit(1);
}

static void	writer(int writer_pid, t_rdrct *rdrct)
{
	if (!writer_pid)
	{
		signal(SIGINT, SIG_IGN);
		if (rdrct->pipe.is)
			close(rdrct->pipe.pipefd[0]);
		if (rdrct->copy.is0)
			close(rdrct->copy.fd[0]);
		if (rdrct->copy.is1)
			close(rdrct->copy.fd[1]);
		if (rdrct->inall.is)
		{
			close(rdrct->inall.pipefd[0]);
			close(rdrct->inall.pipefd[1]);
		}
		close(rdrct->outall.pipefd[1]);
		out(&rdrct->out, rdrct->outall.pipefd[0]);
		close(rdrct->outall.pipefd[0]);
		ft_lstclear(&rdrct->in, NULL);
		ft_lstclear(&rdrct->out, NULL);
		exit(0);
	}
}

static void	postman(int postman_pid, t_rdrct *rdrct)
{
	if (!postman_pid)
	{
		if (rdrct->pipe.is)
			close(rdrct->pipe.pipefd[0]);
		if (rdrct->copy.is0)
			close(rdrct->copy.fd[0]);
		if (rdrct->copy.is1)
			close(rdrct->copy.fd[1]);
		if (rdrct->outall.is)
		{
			close(rdrct->outall.pipefd[0]);
			close(rdrct->outall.pipefd[1]);
		}
		close(rdrct->inall.pipefd[0]);
		if (rdrct->inall.is)
			in(&rdrct->in, rdrct->inall.pipefd[1]);
		close(rdrct->inall.pipefd[1]);
		ft_lstclear(&rdrct->in, NULL);
		ft_lstclear(&rdrct->out, NULL);
		exit(0);
	}
}

int	exec_cmd(char **args, t_rdrct *rdrct)
{
	int	pid;
	int	chpid;

	pid = fork();
	if (!pid)
		child(args, rdrct);
	else
	{
		chpid = fork();
		writer(chpid, rdrct);
		chpid = fork();
		postman(chpid, rdrct);
		if (rdrct->inall.is)
		{
			close(rdrct->inall.pipefd[0]);
			close(rdrct->inall.pipefd[1]);
		}
		close(rdrct->outall.pipefd[0]);
		close(rdrct->outall.pipefd[1]);
		rdrct->outall.is = false;
		rdrct->inall.is = false;
		ft_lstclear(&rdrct->out, NULL);
		ft_lstclear(&rdrct->in, NULL);
	}
	return (pid);
}
