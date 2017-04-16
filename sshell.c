#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * sshell is a shell similar to bash and zsh which can:
 *
 *  1. execute user-supplied commands with optional arguments
 *  2. offer a selection of builtin commands
 *  3. redirect the standard input or standard output of commands to files
 *  4. pipe the output of commands to other commands
 *  5. put commands in the background
 *
 *
 * @return the exit code
 */
int main(int argc, char *argv[])
{
    char *cmd = "/bin/date -u";
    int child_pid;
    child_pid = fork();
    printf("%d\n", child_pid);
    int retval;

    retval = system(cmd);
    fprintf(stdout, "Return status value for '%s': %d\n", cmd, retval);

    return EXIT_SUCCESS;
}