#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

matrix *find_user_fun(char *name, char **args, vari *var, err_ret *error);
char *chk_conf(char *name, char *configPath, err_ret *error);
char *find_path(char *functionName, err_ret *error);
matrix *exec_fun(char *functionPath, char **functionArgs, vari *var, err_ret *error);

/*
 * compare given function name, with file in directory
 */
uint8_t chk_name(char *name, char *dir);

/*
 * search given directory for file matching name
 */
char *search_dir(char *name, char *dir, err_ret *error);

#endif //USER_FUNCTIONS
