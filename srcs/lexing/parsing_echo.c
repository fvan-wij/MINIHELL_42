#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

t_lexer	*default_echo_data(t_lexer *info_list, char **splitted_line)
{
	info_list->input = STDIN_IN;
	info_list->output = STDOUT_OUT;
	info_list->path = get_path_of_command(splitted_line[0]);
	if (!info_list->path)
		return (set_error_lex(info_list, 3, "parsing_echo.c/L10"), NULL);
	return (info_list);
}

t_lexer	*echo_with_meta(t_lexer *info_list, char **splitted_line, \
	int *enum_array, int index)
{
	index = 1;
	info_list = default_echo_data(info_list, splitted_line);
	if (!info_list)
		return (NULL);
	info_list->content = allocate_2d_arr(ft_strlen(splitted_line[0]) + 1, info_list);
	if (!info_list->content)
		return (set_error_lex(info_list, 3, "parsing_echo.c/L23"), NULL);
	ft_strcpy(info_list->content[0], splitted_line[0]);
	info_list->content[1] = ft_calloc(1, sizeof(char));
	if (!info_list->content[1])
		return (set_error_lex(info_list, 3, "parsing_echo.c/L27"), NULL);
	while (splitted_line[index] && \
		is_metacharachter(splitted_line[index][0]) != 1)
	{
		if (splitted_line[index][0] == '"' || splitted_line[index][0] == '\'')
			splitted_line[index] = remove_quotes_string(splitted_line, index);
		if (!splitted_line[index])
			return (set_error_lex(info_list, 3, "parsing_echo.c/L34"), NULL);
		info_list->content[1] = ft_strjoin_and_free(info_list->content[1], \
			splitted_line[index]);
		if (!info_list->content[1])
			return (set_error_lex(info_list, 3, "parsing_echo.c/L37"), NULL);
		if (splitted_line[index + 1] != NULL)
			info_list->content[1] = ft_strjoin_and_free(info_list->content[1], \
				" ");
		if (!info_list->content[1])
			return (set_error_lex(info_list, 3, "parsing_echo.c/L42"), NULL);
		index++;
	}
	info_list = organizing_data(info_list, splitted_line, enum_array, index);
	if (!info_list)
		return (NULL);
	return (info_list);
}

t_lexer	*special_case_echo(t_lexer *info_list, char **splitted_line, \
	int index)
{
	index = 1;
	info_list->content = allocate_2d_arr(ft_strlen(splitted_line[0]) + 1, info_list);
	if (!info_list->content)
		return (set_error_lex(info_list, 3, "parsing_echo.c/L58"), NULL);
	ft_strcpy(info_list->content[0], splitted_line[0]);
	info_list->content[1] = ft_calloc(1, sizeof(char));
	if (!info_list->content[1])
		return (set_error_lex(info_list, 3, "parsing_echo.c/L62"), NULL);
	while (splitted_line[index])
	{
		if (splitted_line[index][0] == '"' || splitted_line[index][0] == '\'')
		{
			splitted_line[index] = remove_quotes_string(splitted_line, index);
			if (!splitted_line[index])
				return (set_error_lex(info_list, 3, "parsing_echo.c/L69"), NULL);
		}
		info_list->content[1] = ft_strjoin_and_free(info_list->content[1], \
			splitted_line[index]);
		if (!info_list->content[1])
			return (set_error_lex(info_list, 3, "parsing_echo.c/L73"), NULL);
		if (splitted_line[index + 1] != NULL)
			info_list->content[1] = ft_strjoin_and_free(info_list->content[1], \
				" ");
		if (!info_list->content[1])
			return (set_error_lex(info_list, 3, "parsing_echo.c/L78"), NULL);
		index++;
	}
	info_list = default_echo_data(info_list, splitted_line);
	if (!info_list)
		return (NULL);
	info_list->next = NULL;
	return (info_list);
}