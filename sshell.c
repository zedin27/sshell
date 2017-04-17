#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

// Function definitions
void executeCommand(char **command, char *commandString);
void printStatusMessage(char *command, int exitcode);
void printPrompt();

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
    // Buffer for user command input
    printPrompt();

    char userInput[512];
    char userInputSplit[512];
    char *command[16];

    while(1) {
        fgets(userInput, 512, stdin);

        // This will fill the rest of the buffer with null, removing any existing characters from previous iterations
        int i = strlen(userInput) - 1;
        while(i < 512) {
            userInput[i++] = '\0';
        }


        // Parse the command by splitting on spaces
        strcpy(userInputSplit, userInput);

        char *arg = strtok(userInputSplit, " ");
        i = 0;
        while (arg != NULL) {
            command[i++] = arg;
            arg = strtok(NULL, " ");
        }

        // This will fill the rest of the buffer with null, removing any existing characters from previous iterations
        while(i < 16) {
            command[i++] = '\0';
        }

        executeCommand(command, userInput);
    }
}

/**
 * Execute a command in a new child process and print a confirmation message to the console
 * @param command
 */
void executeCommand(char **command, char *commandString)
{
    // Create a new child process to run the command
    int pid = fork();

    if(pid == 0) {
        // If the code is running in the original process, execute the command
        execvp(command[0], command);

        // Execution should never reach this point unless command is not found
        fprintf(stderr, "Error: command not found\n");

        // Return code of child process should be 1 if the command is not found
        exit(1);

    } else if(pid > 0) {
        // If code is running in the child process, wait for the command to finish
        wait(&pid);
        printStatusMessage(commandString, WEXITSTATUS(pid));
    } else {
        // There has been an error with the fork
        perror("Error: internal system error");
        exit(1);
    }

    printPrompt();
}

/**
 * Prints a status message to stderr indicating that command execution is finished
 * @param cmd
 */
void printStatusMessage(char *command, int exitcode)
{
    fprintf(stdout, "+ completed '%s' [%d]\n", command, exitcode);
}

/**
 * Print the shell prompt to stdout
 */
void printPrompt()
{
    fprintf(stdout, "sshell$ ");
}