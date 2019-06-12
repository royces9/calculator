#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

err_ret find_user_fun(char *name, char **args, struct vari *var, struct matrix **out);
err_ret chk_conf(char *name, char *configPath, char **out);
err_ret find_path(char *functionName, char **path);
err_ret exec_fun(char *functionPath, char **functionArgs, struct vari *var, struct matrix **out);

/*
 * compare given function name, with file in directory
 */
uint8_t chk_name(char *name, char *dir);

/*
 * search given directory for file matching name
 */
err_ret search_dir(char *name, char *dir, char **out);

#endif //USER_FUNCTIONS
