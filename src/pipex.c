/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeleimat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 11:45:47 by aeleimat          #+#    #+#             */
/*   Updated: 2024/12/08 11:45:51 by aeleimat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
	{
		ft_putstr_fd("pipex: invalid command\n", 2);
		ft_free_tab(s_cmd);
		exit(127);
	}
	path = get_path(s_cmd[0], env);
	if (!path)
	{
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(s_cmd[0], 2);
		ft_free_tab(s_cmd);
		exit(127);
	}
	if (execve(path, s_cmd, env) == -1)
	{
		perror("execve");
		ft_free_tab(s_cmd);
		free(path);
		exit(127);
	}
}

void	child(char **av, int *pipe_fd, char **env)
{
	int	fd;

	fd = open_infile(av[1]);
	if (fd == -1)
	{
		closeing_pipe(pipe_fd);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		closeing_pipe(pipe_fd);
		close(fd);
		exit(0);
	}
	if (dup2(pipe_fd[1], 1) == -1)
	{
		closeing_pipe(pipe_fd);
		close(fd);
		exit(0);
	}
	closeing_pipe(pipe_fd);
	close(fd);
	exec(av[2], env);
}

void	parent(char **av, int *pipe_fd, char **env)
{
	int	fd;

	fd = open_outfile(av[4]);
	if (fd == -1)
	{
		closeing_pipe(pipe_fd);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		closeing_pipe(pipe_fd);
		close(fd);
		exit(0);
	}
	if (dup2(pipe_fd[0], 0) == -1)
	{
		closeing_pipe(pipe_fd);
		close(fd);
		exit(0);
	}
	closeing_pipe(pipe_fd);
	close(fd);
	exec(av[3], env);
}

void	main_error_handel(int err)
{
	if (err == 1)
		perror("pipe");
	if (err == 2)
		perror("fork");
	if (err == 3)
		perror("fork2");
	exit(EXIT_FAILURE);
}

int	main(int ac, char **av, char **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (ac != 5)
		wrong_arg();
	if (pipe(pipe_fd) == -1)
		main_error_handel(1);
	pid = fork();
	if (pid == -1)
		main_error_handel(2);
	if (pid == 0)
		child(av, pipe_fd, env);
	pid = fork();
	if (pid == -1)
		main_error_handel(3);
	if (pid == 0)
		parent(av, pipe_fd, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(NULL);
	wait(NULL);
	return (0);
}
