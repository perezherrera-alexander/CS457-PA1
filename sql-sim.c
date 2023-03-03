#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <sys/stat.h>

#define MAX 200 // An input longer than this will be result in an error.
#define ARG_MAX 100 // Max number of tokens in an input. Both of these values were chosen arbitrarily.

void parseInput(char *input, char *splitWords[]);
int identifyCommand(char *command, char *splitWords[]);
int createDirectory(const char* directoryName);
int createTable(const char* tableName);

void main(int argc, char *argv[]) {
    if (argc > 1){
        printf("Usage: %s <script-to-run>");
    }
    else if (argc == 1) {
        printf("Running script %s", argv[1]);
    }
    else {
        printf("Running in interactive mode");
    }

    //FILE *fptr;
    //fptr = fopen ("test-directory/file_name", "w");
    //fclose(fptr); 
    //int result;
    //result = createDirectory("test-directory");


    char commandInputString[MAX]; // String to hold the input from the user
    char *splitWords[ARG_MAX]; // Array of strings to hold the split words from the input string
    int status = 1; // 1 means failure, 0 means success

    do{
        status = 1; // Reset status. Assume failure until proven otherwise
        printf("sqlight> ");
        fgets(commandInputString, MAX, stdin);

        parseInput(commandInputString, splitWords);
        status = identifyCommand(splitWords[0], splitWords);


    }while(1);

    
    // check if directory is created or not
    /*if (!result)
        printf("Directory created\n");
    else {
        printf("Unable to create directory or directory already exist\n");
        exit(1);
    }
    */
}

void parseInput(char *input, char *splitWords[]){
    int wordInd = 0;
    splitWords[0] = strtok(input, " \n");
    while(splitWords[wordInd] != NULL) {
        splitWords[++wordInd] = strtok(NULL, " \n");
    }
    return;
}

int createDirectory(const char* directoryName) {
    int check;
    check = mkdir(directoryName, 0777);
    return check;
}

int identifyCommand(char *command, char *splitWords[]){
    if (strcmp(command, ".EXIT") == 0) {
        printf("Exiting...\n");
        exit(0);
    }
    else if (strcmp(command, "CREATE") == 0) {
        //printf("Opening database %s");
        if (strcmp(splitWords[1], "DATABASE") == 0) {
            printf("Creating database %s", splitWords[2]);
            createDirectory(splitWords[2]);
            return 0;
        }
        else if (strcmp(splitWords[1], "TABLE") == 0) {
            printf("Creating table %s", splitWords[2]);
        }
        else {
            printf("Invalid command. Please try again.\n");
            return 1;
        }
    }
    else if (strcmp(command, "DROP") == 0) {
        if (strcmp(splitWords[1], "DATABASE") == 0) {
            printf("Dropping database %s", splitWords[2]);
        }
        else if (strcmp(splitWords[1], "TABLE") == 0) {
            printf("Dropping table %s", splitWords[2]);
        }
        else {
            printf("Invalid command. Please try again.\n");
            return 1;
        }
    }
    else if (strcmp(command, "INSERT") == 0) {
        if (strcmp(splitWords[1], "INTO") == 0) {
            printf("Inserting into table %s", splitWords[2]);
        }
        else {
            printf("Invalid command. Please try again.\n");
            return 1;
        }
    }
    else if (strcmp(command, "SELECT") == 0) {
        printf("Selecting from table %s", splitWords[3]);
    }
    else {
        printf("Invalid command. Please try again.\n");
        return 1;
    }
    return 0;
}