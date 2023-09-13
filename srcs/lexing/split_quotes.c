#include "../../includes/minishell.h"
#include "../../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

char	*remove_spaces_quotes_line(char *line)
{
	char	*new_line;
	int		index_n;
	int		index_l;

	index_n = 0;
	index_l = 0;
	new_line = ft_calloc(ft_strlen(line), sizeof(char));
	if (!new_line)
		return (NULL);
	while (line[index_l])
	{
		new_line[index_n] = line[index_l];
		if (line[index_l] == '"' || line[index_l] == '\'')
		{
			index_n++;
			index_l++;
			while (line[index_l] && (line[index_l] != '"' && line[index_l] != '\''))
			{
				if (line[index_l] != ' ')
				{
					new_line[index_n] = line[index_l];
					index_n++;
				}
				index_l++;
			}
			new_line[index_n] = line[index_l];
		}
		index_n++;
		index_l++;
	}
	//free(line);
	return (new_line);
}

char	**store_all_quote_data(char *line, char **temp)
{
	int		index_l;
	int		index_end;
	int		index_temp;

	index_l = 0;
	index_temp = 0;
	while (line[index_l])
	{
		if (line[index_l] == '"' || line[index_l] == '\'')
		{
			index_end = get_end_quote(line, index_l);
			if (index_end == -1)
			{
				free_double_array(temp);
				return (NULL);
			}
			temp[index_temp] = my_random_strcpy(line, index_l, index_end); //I NEED A BETTER NAME FOR THE STRCPY FUNCT
			if (!temp[index_temp])
			{
				free_double_array(temp);
				return (NULL);
			}
			index_l = index_end;
			index_temp++;
		}
		if (line[index_l] != '\0')
		index_l++;
	}
	temp[index_temp] = NULL;
	return (temp);
}

char	**replace_quotes_array(char **split_array, char	**temp_quotes)
{
	int	index;
	int	index_tmp;

	index = 0;
	index_tmp = 0;
	while (split_array[index])
	{
		if (split_array[index][0] == '"' || split_array[index][0] == '\'')
		{
			free(split_array[index]);
			split_array[index] = temp_quotes[index_tmp];
			index_tmp++;
		}
		index++;
	}
	return (split_array);
}

char	**split_with_quotes(char *line, t_lexer *info_list)
{
	char	**split_array;
	char	**temp_quotes;
	int		ammount_quotes;

	ammount_quotes = how_many_quotes(line);
	temp_quotes = ft_calloc(ammount_quotes + 1, sizeof(char *));
	if (!temp_quotes)
		return (set_error_lex(info_list, 3, "split_quotes.c/L97"), NULL);
	temp_quotes = store_all_quote_data(line, temp_quotes);
	if (!temp_quotes)
		return (set_error_lex(info_list, 2, "split_quotes.c/L54"), NULL);
	line = remove_spaces_quotes_line(line);
	if (!line)
		return (set_error_lex(info_list, 3, "split_quotes.c/L14"), NULL);
	split_array = ft_split(line, ' ');
	if (!split_array)
		return (set_error_lex(info_list, 3, "split_quotes.c/L104"), NULL);
	split_array = replace_quotes_array(split_array, temp_quotes);
	return (split_array);
}