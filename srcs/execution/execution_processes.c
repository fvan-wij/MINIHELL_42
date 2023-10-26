/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   execution_processes.c                             :+:    :+:             */
/*                                                    +:+                     */
/*   By: fvan-wij <marvin@42.fr>                     +#+                      */
/*                                                  +#+                       */
/*   Created: 2023/10/23 17:48:36 by fvan-wij      #+#    #+#                 */
/*   Updated: 2023/10/26 11:13:23 by fvan-wij      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

static void	run_cmd(char *path, char *content[], char *env_cpy[])
{
	if (execve(path, content, env_cpy) < 0)
		perror("execve");
}

static void	run_child_process(int in, int *pipe_fd, t_lexer *node,
		t_minishell *shell)
{
	t_builtin	builtin;
	int			err;

	err = -1;
	builtin = is_builtin(node);
	change_signal_profile(CHILD);
	close(pipe_fd[PIPE_READ]);
	route_input(in, node);
	route_output(pipe_fd[PIPE_WRITE], node);
	node->content = colorize_cmd(node->content);
	if (is_absolute_path(node))
		run_cmd(node->path, node->content, shell->env_cpy);
	if (builtin != NO_BUILTIN)
		err = execute_builtin(shell, builtin, node);
	else if (!cmd_exists(node->content[0], shell->env_cpy))
		err = error_command_not_found(node->content[0]);
	else
		run_cmd(node->path, node->content, shell->env_cpy);
	clean_up(shell);
	exit(err);
}

static int	fetch_exit_status(pid_t pid, t_lexer *head,
		char *env_cpy[])
{
	int		status;

	status = 0;
	waitpid(pid, &status, 0); // Make a PID array,
	while (wait(NULL) != -1)
		;
	clean_tmp_files(head, env_cpy);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		return (change_signal_profile(PARENT), 131);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (change_signal_profile(PARENT), 130);
	return (change_signal_profile(PARENT), WEXITSTATUS(status));
}

static pid_t	run_and_route_processes(pid_t pid, t_lexer *head, t_minishell *shell)
{
	t_lexer	*current;
	int		pipe_fd[2];
	int		prev_pipe;

	prev_pipe = STDIN_FILENO;
	current = head;
	while (current)
	{
		if (pipe(pipe_fd) < 0)
			perror("pipe");
		pid = fork(); 
		if (pid < 0)
			return (ft_putstr_fd("Fork failed", STDERR_FILENO), 1);
		change_signal_profile(WAITING);
		if (pid == 0)
			run_child_process(prev_pipe, pipe_fd, current, shell);
		close(pipe_fd[PIPE_WRITE]);
		if (current->cmd_id >= 1)
			close(prev_pipe);
		prev_pipe = pipe_fd[PIPE_READ];
		current = current->next;
	}
	close(prev_pipe);
	return (pid);
}

// static int	cmd_amount(t_lexer *head)
// {
//
// }
//
int	execute_cmds(t_minishell *shell, t_lexer *head, char *envp[])
{
	pid_t	pid;

	pid = 1;
	print_cmd_lst(head);
	if (create_heredoc_loop(head, envp) != 0)
		return (130);
	pid = run_and_route_processes(pid, head, shell);
	return (fetch_exit_status(pid, head, envp));
}
