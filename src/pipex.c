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

void	first_process(int file, int *fd, char *arg, char **envp)
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
	if (execve(path, cmd, envp) == -1)
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	clear_tab(cmd);
	free(path);
}

void	second_process(int file, int *fd, char *arg, char **envp)
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
	if (execve(path, cmd, envp) == -1)
	{
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	clear_tab(cmd);
	free(path);
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
	pid_t	pid[2];
	int		status;

	file[0] = open(argv[1], O_RDONLY);
	if (file[0] == -1)
		return (write(2, "pipex: file1 not found\n", 23), 1);
	file[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipe(fd) == -1 || argc == 1)
		exit(1);
	pid[0] = fork();
	if (pid[0] < 0)
		exit(1);
	if (pid[0] == 0)
		first_process(file[0], fd, argv[2], envp);
	pid[1] = fork();
	if (pid[1] < 0)
		exit(1);
	if (pid[1] == 0)
		second_process(file[1], fd, argv[3], envp);
	close_all(fd, file);
	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
	return (WEXITSTATUS(status));
}
