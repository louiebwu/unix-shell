#ifndef CELL_H
# define CELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sysexits.h>
# include <stdbool.h>
# include <sched.h>
# include <wait.h>

#define Y "\033[1;33m"
#define G "\033[1;32m"
#define C "\033[1;36m"
#define RED "\033[1;31m"
#define RST "\033[0m"

#define p(...) printf(__VA_ARGS__)
#define DEL "\n\t \v\f\r"
#define CELL_Jr 0

typedef struct s_builtin
{
    const char *builtin_name;
    int (*foo)(char **);
}   t_builtin;

#endif