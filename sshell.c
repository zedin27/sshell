#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

// Function definitions
void printStatusMessage(char *command, int exitcode);

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
    // Command to be executed
    char *cmd[3] = {"/bin/date", "-u"};

    // Create a new child process to run the command
    int pid = fork();

    if(pid == 0) {
        // If the code is running in the original process, execute the command
        execvp(cmd[0], cmd);
        perror("execvp");
        exit(1);
    } else if(pid > 0) {
        // If code is running in the child process, wait for the command to finish
        wait(&pid);
        printStatusMessage("/bin/date -u", WEXITSTATUS(pid));
    } else {
        // There has been an error with the fork
        perror("fork");
        exit(1);
    }

    return 0;
}

/**
 * Prints a status message to stderr indicating that command execution is finished
 * @param cmd
 */
void printStatusMessage(char *command, int exitcode)
{
    fprintf(stderr, "+ completed '%s' [%d]\n", command, exitcode);
}