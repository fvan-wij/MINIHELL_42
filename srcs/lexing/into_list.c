#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include <stdio.h>

t_lexer	*one_word_lexer(t_lexer *info_list, char **splitted_line)
{
	info_list->content = splitted_line;
	info_list->path = get_path_of_command(info_list->content[0]);
	info_list->input = STDIN_IN;
	info_list->output = STDOUT_OUT;
	info_list->next = NULL;
	return (info_list);
}

t_lexer	*two_word_lexer(t_lexer *info_list, char **splitted_line)
{
	info_list->content = splitted_line;
	info_list->path = get_path_of_command(info_list->content[0]);
	info_list->input = STDIN_IN;
	info_list->output = STDOUT_OUT;
	info_list->next = NULL;
	return (info_list);
}

t_lexer	*into_linklist(t_lexer *info_list, char *word_var, int enum_var)
{
	int	in_delim;

	if (enum_var == COMMAND)
	{
		info_list->content = allocate_2d_arr(ft_strlen(word_var) + 1, info_list);
		if (!info_list->content)
			return (set_error_lex(info_list, 3, "dataorg_utis.c/L31"), NULL);
		ft_strcpy(info_list->content[0], word_var);
		info_list->path = get_path_of_command(info_list->content[0]);
		// if (!info_list->path)
		// 	return (set_error_lex(info_list, 3, "dataorg_utis.c/L37"), NULL);
	}
	if (enum_var == FLAG)
	{
		info_list->content[1] = ft_calloc(ft_strlen(word_var) + 1, \
			sizeof(char));
		if (!info_list->content[1])
			return (set_error_lex(info_list, 3, "dataorg_utis.c/L41"), NULL);
		ft_strcpy(info_list->content[1], word_var);
	}
	if (enum_var == INFILE || enum_var == OUTFILE)
	{
		info_list->file = ft_calloc(ft_strlen(word_var) + 1, sizeof(char));
		if (!info_list->file)
			return (set_error_lex(info_list, 3, "dataorg_utis.c/L49"), NULL);
		ft_strcpy(info_list->file, word_var);
	}
	if (enum_var == DELIMITER)
	{
		in_delim = info_list->index_delim;
		info_list->delim[in_delim] = ft_calloc(ft_strlen(word_var) + 1, sizeof(char));
		if (!info_list->delim[in_delim])
			return (set_error_lex(info_list, 3, "dataorg_utis.c/L57"), NULL);
		ft_strcpy(info_list->delim[in_delim], word_var);
		info_list->index_delim += 1;
	}
	return (info_list);
}
