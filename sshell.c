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
int isBuiltin(char *command);
void executeBuiltin(char **command, char *commandString);


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
    (void)argc; //UNUSED variable, so casting it to the void to make it readable
        
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

        if(isBuiltin(command[0])) {
            executeBuiltin(command, userInput);
        } else {
            executeCommand(command, userInput);
        }
    }
}

/**
 * Execute a command in a new child process and print a confirmation message to the console
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
        fprintf(stderr, "Error: internal system error");
        exit(1);
    }

    printPrompt();
}

/**
 * Prints a status message to stderr indicating that command execution is finished=
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

/**
 * Returns 1 if the given command is a shell builtin, 0 otherwise
 */
int isBuiltin(char *command)
{
    if (strcmp("exit", command) == 0) return 1;
    if (strcmp("cd", command) == 0) return 1;
    if (strcmp("pwd", command) == 0) return 1;
    return 0;
}

/**
 * Execute a builtin command
 */
void executeBuiltin(char **command, char *commandString)
{
    if(strcmp("exit", command[0]) == 0) {
        fprintf(stderr, "Bye...");
        exit(0);
    } else if(strcmp("pwd", command[0]) == 0) {
        char *dir = getcwd(NULL, 4096);
        fprintf(stdout, "%s\n", dir);

        printStatusMessage(commandString, 0);
    } else if(strcmp("cd", command[0]) == 0) {
        int status = chdir(command[1]);

        if(status != 0) {
            fprintf(stdout, "Error: no such directory\n");
            printStatusMessage(commandString, 1);
        } else {
            printStatusMessage(commandString, 0);
        }

    }

    printPrompt();
}
