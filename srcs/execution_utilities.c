#include "../includes/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	*get_path(char *cmd)
{
	int		i;
	char	**paths;
	char	*temp;

	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], ft_strjoin("/", cmd));
		if (access(temp, X_OK) == 0)
			return (temp);
		else
		 free(temp);
		i++;
	}
	return (NULL);
}

int	check_access(char *cmd)
{
	int		i;
	char	**paths;
	char	*temp;

	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], ft_strjoin("/", cmd));
		if (access(temp, X_OK) == 0)
			return (1);
		else
			free(temp);
		i++;
	}
	return (-1);
}

int	arg_is_env(char *raw_input, char **value)
{
	char	*temp;

	temp = ft_strdup(raw_input + 1);
	*value = getenv(temp);
	if (raw_input[0] == '$' && *value)
		return (1);
	else
	{
		free(temp);
		*value = NULL;
		return (0);
	}
}
