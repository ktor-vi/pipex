/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vphilipp <vphilipp@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 13:39:12 by vphilipp          #+#    #+#             */
/*   Updated: 2023/11/20 13:39:14 by vphilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	child_process(int file, int *fd, char *arg, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = parse_arg(arg);
	path = get_cmdpath(cmd, envp);
	dup2(file, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(file);
	close(fd[1]);
	if (execve(path, cmd, NULL) == -1)
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	clear_tab(cmd);
	free(path);
}

void	parent_process(int file, int *fd, char *arg, char **envp)
{
	char	**cmd;
	char	*path;

	cmd = parse_arg(arg);
	path = get_cmdpath(cmd, envp);
	dup2(file, STDOUT_FILENO);
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(file);
	close(fd[0]);
	if (execve(path, cmd, NULL) == -1)
	{
		ft_putstr_fd("parentp: command not found\n", 2);
		exit(127);
	}
	clear_tab(cmd);
}

void	close_all(int *fd, int *file)
{
	close(fd[0]);
	close(fd[1]);
	close(file[0]);
	close(file[1]);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	int		file[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;

	file[0] = open(argv[1], O_RDONLY);
	file[1] = open(argv[4], O_RDWR);
	if (pipe(fd) == -1 || argc == 1)
		exit(1);
	pid = fork();
	if (pid < 0)
		exit(1);
	if (pid == 0)
		child_process(file[0], fd, argv[2], envp);
	pid2 = fork();
	if (pid2 < 0)
		exit(1);
	if (pid2 == 0)
		parent_process(file[1], fd, argv[3], envp);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}
