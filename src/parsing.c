/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vphilipp <vphilipp@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:39:04 by vphilipp          #+#    #+#             */
/*   Updated: 2023/11/20 13:39:06 by vphilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <unistd.h>

char	**parse_arg(char *arg)
{
	char	**command;

	command = ft_split(arg, ' ');
	return (command);
}

void	clear_tab(char **splitted)
{
	int	k;

	k = 0;
	while (splitted[k])
	{
		free(splitted[k]);
		k++;
	}
	free(splitted);
}

char	*get_cmdpath(char **cmd, char **envp)
{
	int		i;
	int		j;
	char	*path;
	char	**envs;
	char	*temp_env;

	i = 0;
	j = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	path = ft_substr(envp[i], 5, ft_strlen(envp[i]));
	envs = ft_split(path, ':');
	while (envs[j])
	{
		temp_env = ft_strjoin(envs[j], "/");
		temp_env = ft_strjoin(temp_env, cmd[0]);
		if (access(temp_env, X_OK) == 0)
			return (temp_env);
		free(temp_env);
		j++;
	}
	free(envs);
	free(path);
	return (NULL);
}
