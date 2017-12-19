////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Author: Sherman Chao Wen Chow
//  Date: September 17, 2017
//
//  Simple Shell
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Define sizes
#define MAX_BUFFER_LENGTH 1024
#define MAX_TOKEN_LENGTH 200

// This function prompts the user to enter commands
// Shell prompt is in the format: time followed by hash sign (e.g. 12:12:12#)
void promptCommand()
{
    char s[1000];
    time_t currTime;

    time(&currTime);
    struct tm *timeInfo = localtime(&currTime);

    strftime(s, 1000, "%H:%M:%S", timeInfo);  // Edit format of timeInfo to show time
    printf("%s# ", s);
}

// This function reads in a command and breaks the string into tokens, storing a pointer to each token in "tokens[]"
void readCommand(char *buffer, char *tokens[])
{
    int i = 0;
    int numberOfTokens = 0;
    char *p;

    // Read in command from stdin
    fgets(buffer, 200, stdin);
    // Error handling
    if (buffer == NULL)
    {
        printf("ERROR: Failed to read command\n");   // Error message
        exit(-1); // Unsuccessful termination
    }

    // Tokenize string
    p = strtok(buffer, " \n");
    while (p != NULL)
    {
        tokens[i++] = p;
        numberOfTokens++;   // Increment the number of tokens
        p = strtok(NULL, " \n");
    }
    // Set last token after the user arguments as NULL
    tokens[numberOfTokens] = NULL;
}

// This function runs a command
// "exit" command terminates shell
// Uses fork() to create new child process, execvp() to execute command, and waitpid() to wait for child process command to finish running
void runCommand(char *tokens[])
{
    char cwd[MAX_BUFFER_LENGTH];
    char *printWorkingDir;
    int changeDir;
    int executeResult;
    pid_t pid;
    pid_t waitingPid;
    int status;

    // Terminate the shell if user enters "exit"
    if (strcmp(tokens[0], "exit") == 0)
    {
        exit(0);    // Successful termination
    }
    // Display working directory if user enters "pwd"
    else if (strcmp(tokens[0], "pwd") == 0)
    {
        printWorkingDir = getcwd(cwd, sizeof(cwd));
        // Error handling
        if (printWorkingDir == NULL)
        {
            printf("ERROR: Failed to print working directory\n");  // Error message
        }
        printf("%s\n", cwd);
    }
    // Change directory if user enters "cd <directory>"
    else if (strcmp(tokens[0], "cd") == 0)
    {
        changeDir = chdir(tokens[1]);
        // Error handling
        if (changeDir == -1)
        {
            printf("%s: %s: No such file or directory\n", tokens[0], tokens[1]);  // Error message
        }
    }
    // Otherwise
    else
    {
        // Create child process
        /*------------------------------------------------------------------------------
        fork() - creates child process, return value is of type pid_t
            - Returns pid == 0: returns a zero to newly created child process
            - Returns pid > 0: returns positive value, pid of child process, to parent
            - Returns pid == -1: creation of child process was unsuccessful
        ------------------------------------------------------------------------------*/
        pid = fork();
        if (pid == 0)  // return 0 to newly created child process
        {
            // Run command
            /*------------------------------------------------------------------------------
            execvp(const char *file, char *const argv[]) - Replaces current process image with new process image
                - "file" is the file to execute
                - "argv[]" is array of arguments
                - Returns -1 if unsuccessful
            ------------------------------------------------------------------------------*/
            executeResult = execvp(tokens[0], tokens);
            // Error handling
            if (executeResult == -1)
            {
                // Invalid command
                printf("ERROR: Command not found\n");    // Error message
                exit(-1); // Unsuccessful termination
            }
        }
        else if (pid > 0)    // return positive value, pid of child process, to parent
        {
            // Parent process must wait for child process command to finish running
            /*------------------------------------------------------------------------------
            waitpid(pid_t pid, int *statusPtr, int options)
                - "pid" is process of the child it should wait
                - "statusPtr" is a pointer to the location where status info for terminating process it be stored
                - "options" spcifies optional actions for waitpid function
                    - Returns pid of child when it is terminated
                    - Returns -1 if unsuccessful
            ------------------------------------------------------------------------------*/
            waitingPid = waitpid(pid, &status, 0);
            // Error handling
            if (waitingPid == -1)
            {
                printf("ERROR: Failed to wait for child process\n"); // Error message
                exit(-1); // Unsucessful termination
            }
        }
        // Error handling
        else if (pid == -1)    // Creation of child process unsucessful
        {
            printf("ERROR: Failed to create child process\n"); // Error message
            exit(-1); // Unsuccessful termination
        }
    }
}

// Main function
int main(void)
{
    char buffer[MAX_BUFFER_LENGTH];
    char *tokens[MAX_TOKEN_LENGTH];

    // Infinite while loop to start shell, enter "exit" to terminate shell
    while (1)
    {
        // Call function to prompt user to enter command
        promptCommand();

        // Call function to tokenize string
        readCommand(buffer, tokens);

        // No commands entered
        if (tokens[0] == NULL || strcmp(tokens[0], "\n") == 0 || strcmp(tokens[0], "\t") == 0)
            continue;

        // Call function to run command
        // Uses fork(), execvp(), and waitpid()
        runCommand(tokens);
    }

    return 0;
}
