#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void	free_ll(t_lexer **lst)
{
	t_lexer	*current;

	current = *lst;
	while (current != NULL)
	{
		current = *lst;
		if (current->path)
			free(current->path);
		if (current->file)
			free(current->file);
		if (current->delim)
			free(current->delim);
		if (current->content)
			free_double_array(current->content);
		current = current->next;
		free(*lst);
		*lst = current;
	}
	free(*lst);
}

void	clean_up(t_minishell *shell)
{
	free_double_array(shell->env_cpy);
	free_ll(&shell->cmd_lst);
}