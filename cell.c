#include "cell.h"
#include "utils.c"
#include "builtin.c"

t_builtin g_builtin[] =
{
    {.builtin_name = "echo", .foo=cell_echo}, 
    {.builtin_name="env", .foo=cell_env},
    {.builtin_name="exit", .foo=cell_exit},
    {.builtin_name=NULL},
};

int status = 0;

void cell_launch(char **args)
{
    if (Fork() == CELL_Jr)
        Execvp(args[0],args);
    else
        Wait(&status);
}

void cell_exec(char **args)
{
    if (!strcmp(args[0], "cd")) 
    {
        if (!args[1]) 
            Chdir(getenv("HOME")); 
        else 
            Chdir(args[1]);
    return;
    }       
    
    int i;
    const char *curr;

	if (!args || !args[0])
		return ;
        
    i = 0;
    while ((curr = g_builtin[i].builtin_name))
    {
        if (!strcmp(curr, args[0]))
        {
            status = g_builtin[i].foo(args);
            return;
        }
        i++;
    }
    cell_launch(args);
}

char **cell_split_line(char *line)
{
    char **tokens;
    unsigned int position;
    size_t bufsize;

    bufsize = BUFSIZ;
    tokens = Malloc(bufsize * sizeof *tokens);
    position = 0;
    
    for (char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
    {
        tokens[position++] = token;
        if (position >= bufsize)
        {
            bufsize *= 2;
            tokens = Realloc(tokens, bufsize * sizeof(*tokens));
        }
    }

    tokens[position] = NULL;
    return tokens;
}

char *cell_read_line(void)
{
    char    *buf;
    size_t  bufsize;
    char    cwd[BUFSIZ];
    
    buf = NULL;

    Getcwd(cwd, sizeof(cwd));
    p(C "%s" RST " $> ", cwd);

    if (getline(&buf, &bufsize, stdin) == -1)
    {
        buf = NULL;
        if (feof(stdin))
            p(RED"[EOF]"RST);
        else
            p(RED"Getline Failed"RST);
    }
    return buf;
}

int main()
{
    char    *line;
    char    **args;
    while (line = cell_read_line())
    {
		args = cell_split_line(line);	

		if (args[0] && !strcmp(args[0], "cd"))
			Chdir(args[1]);
		
		cell_exec(args);

        free(line);
		free(args);

    }

    return EXIT_SUCCESS;
}