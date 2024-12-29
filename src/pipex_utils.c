/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 11:46:06 by aeleimat          #+#    #+#             */
/*   Updated: 2024/12/08 11:46:08 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_infile(char *file)
{
	int	fd;

	if (access(file, F_OK) == -1)
	{
		write(2, "No such file or directory\n", 27);
		return (-1);
	}
	if (access(file, R_OK) == -1)
	{
		write(2, "Permission denied\n", 18);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open infile");
		return (-1);
	}
	return (fd);
}

int	open_outfile(char *file)
{
	int	fd;

	if (access(file, F_OK) == -1)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else
	{
		if (access(file, W_OK) == -1)
		{
			write(2, "Permission denied\n", 18);
			return (-1);
		}
		fd = open(file, O_WRONLY | O_TRUNC, 0664);
	}
	if (fd == -1)
	{
		perror("open outfile");
		return (-1);
	}
	return (fd);
}

void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*get_the_path(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	i = -1;
	allpath = ft_split(get_the_path("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (cmd);
}
