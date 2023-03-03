#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <sys/stat.h>

#define MAX 200 // An input longer than this will result in an error.
#define ARG_MAX 100 // Max number of tokens in an input. Both of these values were chosen arbitrarily.

// Input parsing functions
void parseInput(char *input, char *splitWords[]);
int identifyCommand(char *command, char *splitWords[]);
// Command functions
int createCommand(char *splitWords[]);
int dropCommand(char *splitWords[]);
int useCommand(char *splitWords[]);
int selectCommand(char *splitWords[]);
int alterCommand(char *splitWords[]);
// Auxiliary functions
int createDirectory(const char* directoryName);
int createTable(const char* tableName);


void main(int argc, char *argv[]) {
    if (argc > 1) printf("Usage: %s <script-to-run>");
    else if (argc == 1) printf("Running script %s", argv[1]);
    else printf("Running in interactive mode");

    char commandInputString[MAX]; // String to hold the input from the user
    char *splitWords[ARG_MAX]; // Array of strings to hold the split words from the input string
    int status = 1; // 0 means success, 1 means failure

    do{
        status = 1; // Reset status.
        printf("sqlight> ");
        fgets(commandInputString, MAX, stdin);

        parseInput(commandInputString, splitWords);
        status = identifyCommand(splitWords[0], splitWords);
        // While I'm going to track the status, I plan to have the functions print their own error messages. Maybe this'll be useful later.
    }while(1);
}

void parseInput(char *input, char *splitWords[]){
    int wordInd = 0;
    splitWords[0] = strtok(input, " \n");
    while(splitWords[wordInd] != NULL) {
        splitWords[++wordInd] = strtok(NULL, " \n");
    }
    return;
}

int identifyCommand(char *command, char *splitWords[]){
    int status = 1;
    if (strcmp(command, "CREATE") == 0) {
        status = createCommand(splitWords);
    }
    else if (strcmp(command, "DROP") == 0) {
        status = dropCommand(splitWords);
    }
    else if (strcmp(command, "SELECT") == 0) {
        status = selectCommand(splitWords);
    }
    else if (strcmp(command, "USE") == 0) {
        status = useCommand(splitWords);
    }
    else if (strcmp(command, "ALTER") == 0) {
        status = alterCommand(splitWords);
    }
    else if (strcmp(command, ".EXIT") == 0) {
        printf("All Done!\n");
        exit(0); // No need to return a status. Just exit.
    }
    else {
        printf("Errror: Invalid command. Please try again.\n");
        return 1;
    }
    return status;
}

int createCommand(char *splitWords[]){
    if (strcmp(splitWords[1], "DATABASE") == 0) {
        printf("Creating database %s", splitWords[2]);
        createDirectory(splitWords[2]);
        
    }
    else if (strcmp(splitWords[1], "TABLE") == 0) {
        printf("Creating table %s", splitWords[2]);
        createTable(splitWords[2]);
    }
    else {
        printf("Error: Invalid syntax. Please try again.\n");
        return 1;
    }
    return 0;
}

int createDirectory(const char* directoryName) {
    int check;
    check = mkdir(directoryName, 0777);
    return check;
}

int createTable(const char* tableName) {

    //FILE *fptr;
    //fptr = fopen ("test-directory/file_name", "w");
    //fclose(fptr); 
    //int result;
    //result = createDirectory("test-directory");

    return -1;
}