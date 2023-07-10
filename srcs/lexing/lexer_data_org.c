#include "../includes/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

t_lexer	*organizing_data(t_lexer *info_list, char **splitted_line, int *enum_array)
{
	int		index;
	int		check;
	t_lexer	*head;

	index = 0;
	head = info_list;
	while (splitted_line[index])
	{
		if (enum_array[index] == COMMAND)
		{
			info_list = into_linklist(info_list, splitted_line[index], enum_array[index]);
			if (!info_list)
				return (NULL);
			index++;
			check = check_for_flags(splitted_line, enum_array, index);
			if (check > 0)
				info_list = into_linklist(info_list, splitted_line[check], enum_array[check]);	
		}
		if (enum_array[index] == PIPE)
		{
			if (info_list->input != INFILE)
				info_list->input = STDIN_IN;	
			info_list->output = PIPE_WRITE;
			info_list = create_new_node(info_list);
			info_list->input = PIPE_READ;
			if (info_list->input != OUTFILE)
				info_list->output = STDOUT_OUT;
		}
		if (enum_array[index] == INFILE || enum_array[index] == OUTFILE)
		{
			info_list = into_linklist(info_list, splitted_line[index], enum_array[index]);
			if (!info_list)
				return (NULL);
			if (enum_array[index] == INFILE)
				info_list->input = INFILE;
			if (enum_array[index] == OUTFILE)
				info_list->output = OUTFILE;
		}
		if (enum_array[index] == DELIMITER)
		{
			if (splitted_line[index + 1])
				info_list = into_linklist(info_list, splitted_line[index + 1], enum_array[index]);
			if (!info_list)
				return (NULL);
			index++;
		}
		if (splitted_line[index] != NULL)
			index++;
	}
	info_list->next = NULL;
	free_2d_array(splitted_line);
	return (head);
}