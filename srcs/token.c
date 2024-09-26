/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razouani <razouani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 17:59:59 by razouani          #+#    #+#             */
/*   Updated: 2024/09/26 20:33:41 by razouani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	creat_node(char *type, t_token *token, char *value)
{
	token->type = ft_strdup(type);
	token->value = ft_strdup(value);
	token->next = ft_calloc(sizeof(t_token), 1);
}

static void grap_mot(t_minishell *minishell, int *index)
{
	int i;
	int len;
	int j;

	i = *index;
	j = 0;
	while(minishell->buffer[i] != ' ' && minishell->buffer[i] != '\t' && minishell->buffer[i])
		i++;
	len = i - *index;
	if (len <= 0)
		return;
	minishell->current = ft_calloc(sizeof(char), len + 1);
	while (*index < i && minishell->buffer[*index])
	{
		minishell->current[j] = minishell->buffer[*index];
		j++;
		*index += 1;
	}
	minishell->current[*index] = '\0';
	return;
}



static int	get_type(char *mot, t_token *token, t_pipex *pipex)
{
	if (chdir(mot) == 0)
		return (creat_node("dossier", token, mot), 0);
	else if (search_command_for_token(pipex, mot) == 0)
		return (creat_node("commande", token, mot), 0);
	else if (ft_strcmp(mot, ">") == 0)
		return (creat_node("redirect output", token, mot), 0);
	else if (ft_strcmp(mot, "<") == 0)
		return (creat_node("redirect input", token, mot), 0);
	return (1);
	
}

static int count_chef(char *mot)
{
	int i;
	int c;
	char chef;

	i = 0;
	c = 0;
	while(mot[i])
	{
		if ((mot[i] == 34 || mot[i] == 39) && (c == 0))
			chef = mot[i];
		if (mot[i] == chef)
			c++;
		i++;
	}
	return (c);
}

static char *dans_cot(char *mot, int chef)
{
	int i;
	int y;
	char *clear_mot;
	char cot;

	i = 0;
	y = 0;
	cot = mot[i];
	clear_mot = ft_calloc(sizeof(char), (ft_strlen(mot) - chef) + 1);
	while(mot[i])
	{
		if (mot[i] == cot)
			i++;
		clear_mot[y] = mot[i];
		i++;
		y++;
	}
	return(clear_mot);
}

static void get_double_cot(char *mot, t_token *token, t_pipex *pipex, int chef)
{
	
	int i;
	char *in_cot;
	int y;
	int c;
	
	i = 0;
	y = 0;
	c = 0;
	in_cot = dans_cot(mot, chef);
	ft_printf(": ||%s||\n", in_cot);
	while(in_cot[i])
	{
		if (mot[i] == ' ')
			c++;
		i++;
	}
	if(c == 0)
	{
		if (get_type(mot, token, pipex) == 0)
			return;
		else
			creat_node("string", token, mot);
	}
	
	/*
		fait ne sorte que la fonction comprenne le cas "l"s"" utilise la fonctione juste au dessus
	*/
}


int	tokenisation(t_token *token, t_minishell *minishell, t_pipex *pipex)
{
	int i;
	//t_token *tmp = token;
	// int y = 0;
	pipex->path = pipex->ev;
	
	i = 0;
	while(minishell->buffer[i])
	{
		while((minishell->buffer[i] == ' ' || minishell->buffer[i] == '\t') && (minishell->buffer[i]))
			i++;
		grap_mot(minishell, &i);
		if (count_chef(minishell->current) != 0)
			get_double_cot(minishell->current, token, pipex, count_chef(minishell->current));
		get_type(minishell->current, token, pipex);
		// ft_printf("le type: %s\n", token->type);
		// ft_printf("le value: %s\n", token->value);
		token = token->next;
	}
	return (EXIT_SUCCESS);	
}



