#ifndef MINISHELL_H 
# define MINISHELL_H 

//###############################################################
//DATA_STRUCTURES
//###############################################################

//Global variable - is equal to the most recent exit status of the shell.

extern int g_exit_status;

//Token enumerator - defines the possible types of tokens;
typedef enum e_token{
	PIPE_READ, //0
	PIPE_WRITE, //1 
	INFILE, //2
	OUTFILE, //3
	COMMAND, //4
	FLAG, //5
	PIPE, //6
	ENV_VAR, //7
	DELIMITER, //8
	APPENDER, //9
	REDIRECT_IN, //10
	REDIRECT_OUT, //11
	NONE, //12
	APPEND, //13
	STDIN_IN, //14
	STDOUT_OUT, //15
	HEREDOC, //16
} t_token;

//Builtin enumerator - defines the different types of possible builtins;
typedef enum e_builtin {
	ECHO,
	CD,
	PWD,
	ENV,
	EXIT,
	EXPORT,
	UNSET,
	NO_BUILTIN,
} t_builtin;

//Linked list node - each command is parsed into a t_lexer node for the executioner to be handled accordingly;
typedef struct lexerinfo {
	char				**content; 		//INFILE/OUTFILE/CMD + OPTIONAL FLAGS (file could also be absolute/relative path);
	char				*path;			//IF NULL -> NO PATH, ELSE -> PATH TO CMD;
	char				*file;			//IF THERE IS AN INFILE OR OUTFILE, ELSE NULL;
	int					input;			//ENUM_IO:PIPE_READ/FILE/STDIN/NONE
	int					output;			//ENUM_IO:PIPE_WRITE/FILE/STDOUT/NONE
	char				**delim;
	int					index_delim;
	int					check_free;
	int					error_code;
	char				**env_copy;
	const char			*error_str;
	struct lexerinfo	*next;
}	t_lexer;

//ERROR CODES:
//1 = COSTUM MESSAGE
//2 = SYNTAX_ERROR
//3 = MALLOC_ERROR

//Main struct - contains all the meta data about minishell such as environmental variables, access to the t_lexer list, status, builtin token and more.
typedef struct s_minishell {
	t_lexer				*cmd_lst;
	char				**env_cpy;
	int					status;
	int					builtin;
}	t_minishell;

//###############################################################
//		LEXER FUNCTIONS
//###############################################################

		//lexer.c
t_lexer	*lexing(char *line, char **env_cpy);
t_lexer	*which_case(t_lexer	*info_list, char **splitted_line, int *enum_array);
t_lexer	*parsing_array(t_lexer *info_list, char **splitted_line, int *enum_array);

		//line_checker.c
int		input_line_check(char *line, t_lexer *info_list);

		//linechecker_checks.c
int		check_quotes(char *line, int index, t_lexer *info_list);
int		check_outredirect(char *line, int index, t_lexer *info_list);
int		check_inredirect(char *line, int index, t_lexer *info_list);
int		check_pipe(char *line, int index, t_lexer *info_list);

		//enum_arrayfts.c
int		*into_enum_array(char **splitted_line, int *enum_array, int index);
int		which_enum(char **splitted_line, int index);

		//lexer_utils.c
int		skip_spaces(char *line, int index);
int		ammount_of_words(char *line);
int		get_max_array(char **array);
int		check_for_quotes(char *line);
int		check_spaces_in_quotes(char *line);

		//eddit_line.c
char	*put_spaces_in_line(char *line, t_lexer *info_list);
char	*edit_line(char *old, char *new);
int		is_metacharachter(char c);

		//split_quotes.c
char	**split_with_quotes(char *line, t_lexer *info_list);
char	**split_intoarray(char *line, t_lexer *info_list, char **temp_quotes);
char	**replace_quotes_array(char **split_array, char	**temp_quotes);

		//splitquo_quotefts.c
char	*remove_spaces_quotes_line(char *line, char *new_line, int index_n, int index_l);
char	**store_all_quote_data(char *line, char **temp);

		//split_quotes_utils.c
int		how_many_quotes(char *line);
int		get_end_quote(char *line, int end, int which);
char	*strcpy_splitquo(char *line, int begin, int end);

		//get_path.c
char	*get_path_of_command(char *command);
char	*find_path_loop(char **paths, char *command);
char	**put_slash_behind(char **paths);

		//data_org.c
t_lexer	*organizing_data(t_lexer *info_list, char **splitted_line, int *enum_array, int index);

		//dataorg_cases.c
t_lexer	*data_org_command(t_lexer *info_list, char **splitted_line, int *enum_array, int index);
t_lexer	*data_org_pipe(t_lexer *info_list);
t_lexer	*data_org_file(t_lexer *info_list, char **splitted_line, int *enum_array, int index);
t_lexer	*data_org_delim(t_lexer *info_list, char **splitted_line, int *enum_array, int index);
t_lexer *data_org_appender(t_lexer *info_list, char **splitted_line, int index);

		//dataorg_utils.c
t_lexer *create_new_node(t_lexer *info_lexer);
char	**allocate_2d_arr(int size, t_lexer *info_list);
int		check_for_flags(char **splitted_line, int *enum_arr, int index);
int		check_for_outfile(char **splitted_line, int *enum_array, int index);

		//data_org_special.c
int		check_special_cases(char **splitted_line);
t_lexer *which_special_case(t_lexer *info_list, char **splitted_line, int *enum_array);
t_lexer	*special_case_rm(t_lexer *info_list, char **splitted_line, int index);

		//into_list.c
t_lexer	*one_two_word_lexer(t_lexer *info_list, char **splitted_line);
t_lexer	*into_linklist(t_lexer *info_list, char *word_var, int enum_var);

		//parsing_grep.c
int		check_for_grep(t_lexer *info_list);
t_lexer *grep_parser(t_lexer *info_list, char **splitted_line);
t_lexer	*rm_quotes_grep(t_lexer *info_list);

		//parser_cat.c 
int		check_for_cat(t_lexer *info_list);
t_lexer *cat_parser(t_lexer *info_list, char **splitted_line);
t_lexer *check_content(t_lexer *info_list, char **splitted_line, int index);

		//parsing_echo.c
t_lexer	*special_case_echo(t_lexer *info_list, char **splitted_line);
t_lexer	*spca_echo_intolist(t_lexer *info_list, char **splitted_line, int index);
t_lexer	*echo_with_meta(t_lexer *info_list, char **splitted_line, int *enum_array);
t_lexer	*echo_meta_intolist(t_lexer *info_list, char **splitted_line, int index);
t_lexer	*default_echo_data(t_lexer *info_list, char **splitted_line);

		//variable_expander.c
char	**replace_var_expander(t_lexer *info_list, char **splitted_line, char **env_cpy);
char	*get_env_var(char *line, char **env_cpy, int ammount_env);
char	*replace_variables(char *line, char **env_temp);

		//varexp_arrayft.c
char	**expand_env_variables(char **env_temp, char **env_cpy);
char	*expand_variable(char *line, char **env_cpy);
char	**fill_array_env(char *line, int ammount_env, char **env_temp, int index);
char	**check_quotes_env(char **splitted_line);

		//varexp_lineft.c
char	*get_variable(char *line, int begin, int end);
char	*remove_quotes_string(char **splitted_line, int index);
char	*remove_dollar_sign(char *line);
char	*put_env_in_line(char *new_line, int index_l, char **env_temp, int index_env);

		//varexp_utils.c
int		how_many_env_var(char *line);
int		get_env_end(char *line, int index);
int		get_size_strings(char *line, char **env_temp);
int		check_for_envvar(char **splitted_line);

		//delimiter_func.c
int		get_number_delim(int *enum_array);

//###############################################################
//		ERROR AND FREE FUNCTIONS
//###############################################################

		//error.c
void	error_command_not_found(char *cmd);
void	error_export_invalid_identifier(char *input);
void	error_unset_too_few_args();
void	error_lexing_message(t_lexer *list);
void	error_lex(t_lexer *info_list, int error_code, const char *str);

		//free.c
void	free_lexing_content_struct(t_lexer *list);
void	free_lexing_struct(t_lexer *list);

//###############################################################
//		EXECUTION FUNCTIONS
//###############################################################

//		execution_processes.c
int		execute_cmds(t_minishell *shell, t_lexer *head, char *envp[]);

//		execution_utilities.c
char	*get_path(const char *cmd);
int		arg_is_env(char *raw_input, char **value, char *envp[]);
int		check_access(char *cmd);
int		var_exists(char	**env, char *var);
char	*ft_getenv(char *var_name, char **env);

//		utilities_double_arrays.c
char	**copy_double_array(char **array);
void	print_double_array(char **arr);
void	free_double_array(char **array);
char	**append_to_double_array(char **src, char *str);
char	**replace_str_in_array(char **src, char *str, int index);
char	**remove_str_from_array(char **src, int index);

//		utilities_debugging.c
void	print_cmd_lst(t_lexer *head);
void	printing_lexer(t_lexer *info_lexer);

//		execution_cd.c
void	execute_cd(t_minishell *shell);
//		execution_echo.c
void	execute_echo(char **raw_input, char *envp[]);
//		execution_env.c
void	execute_env(char *envp[]);
//		execution_export.c
void	execute_export(t_minishell *shell);
//		execution_pwd.c
void	execute_pwd();
//		execution_unset.c
void	execute_unset(t_minishell *shell);

//		interface_frontend.c
void	init_ascii_art(void);
void	remove_ctl_echo();

//		execution_builtin.c
int		execute_builtin(t_minishell *shell, t_builtin builtin);
int		is_builtin(t_lexer *cmd_lst);

//		execution_heredoc.c
void	create_heredoc_loop(t_lexer *head, char *env_cpy[]);
void	clean_tmp_files(t_lexer *head, char *envp[]);

//		execution_heredoc_expansion.c
char	*expand_heredoc_var(char *heredoc_line, int var_index, char *env_cpy[]);
char	*expand_heredoc_pid(char *heredoc_line, int pid_token_index);

//		execution_routing.c
void	route_output(int out, t_lexer *node);
void	route_input(int in, t_lexer *node);
void	redirect_from_to(int fd_from, int fd_to);

//		signal_handling.c
void	catch_signals_parent(void);
void	catch_signals_child(void);

//		memory_management.c		
void	clean_up(t_minishell *shell);
void	free_ll(t_lexer **lst);

//###############################################################


#endif
