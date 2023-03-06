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
int identifyCommand(char *command, char *splitWords[]); // A little confusingly, this is to identify commands, not for a command called "identify".
// Command functions
int createCommand(char *splitWords[], int parameterCount);
int dropCommand(char *splitWords[]);
int useCommand(char *splitWords[]);
int selectCommand(char *splitWords[]);
int alterCommand(char *splitWords[]);
// Auxiliary functions
int createDirectory(const char* directoryName);
int createTable(char* tableName, char* splitWords[], int parameterCount);
int deleteTable(const char* tableName);

void cleanTableParameters(char *splitWords[], char *tableParameters[], int parameterCount);
void removeChar(char *str, char garbage);

int checkUse();

char workingPath[100]; // Stores the path to the current working directory (database).
int useCheck = 0; // 0 means no database is in use, 1 means a database is in use.

void main(int argc, char *argv[]) {
    if (argc > 2) printf("Usage: ./sql-sim <script-to-run>");
    else if (argc == 2) printf("Running script %s\n", argv[1]);
    else printf("Running in interactive mode\n");

    char commandInputString[MAX]; // String to hold the input from the user
    char *splitWords[ARG_MAX]; // Array of strings to hold the split words from the input string
    int status = 1; // 0 means success, 1 means failure

    do{
        status = 1; // Reset status.
        printf("sqlite> ");
        fgets(commandInputString, MAX, stdin);
        parseInput(commandInputString, splitWords);
        //list the tokens
        //for(int i = 0; i < ARG_MAX; i++) {
        //    if(splitWords[i] != NULL) printf("%s\n", splitWords[i]);
        //    else break;
        //}
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
    //printf("From identifyCommand\n");
        // list the tokens
        //for(int i = 0; i < ARG_MAX; i++) {
        //    if(splitWords[i] != NULL) printf("%s\n", splitWords[i]);
        //    else break;
        //}
    int status = 1;

    if (strcmp(command, ".EXIT") == 0) {
        printf("All Done!\n");
        exit(0); // No need to return a status. Just exit.
    }

    // Remove semicoln at end of input
    int tokenCount = 0;
    for(int i = 0; i < ARG_MAX; i++) { // Find the number of tokens in the input
            if(splitWords[i] != NULL) tokenCount++;
            else break;
        }
    int lastTokenLength = strlen(splitWords[tokenCount-1]); // Find the length of the last token
    //char temp[lastTokenLength-1];
    if(splitWords[tokenCount-1][lastTokenLength-1] == ';') {
        removeChar(splitWords[tokenCount-1], ';');
        //for(int i = 0; i < lastTokenLength-1; i++) { // Copy the last token into a new string, minus the semicolon
        //temp[i] = splitWords[tokenCount-1][i]; 
        //}
        //strcpy(splitWords[tokenCount-1], temp);
    }
    else { // If there is no semicolon, print an error and return.
        printf("!Error: Semicolon Missing\n");
        return 1;
    }
    //memset(temp,0,strlen(temp)); // Clear the temp string
    printf("tokenCount: %d\n", tokenCount);
    printf("lastTokenLength: %d\n", lastTokenLength);
    

    if (strcmp(command, "CREATE") == 0) {
        int parameterCount = tokenCount-3;
        printf("(identifyCommand) parameterCount: %d\n", parameterCount);
        status = createCommand(splitWords, parameterCount);
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
    else {
        printf("Errror: Invalid command. Please try again.\n");
        return 1;
    }
    return status;
}

int createCommand(char *splitWords[], int parameterCount) {
    int status = 1;
        //printf("From createCommand: \n");
        // list the tokens
        //for(int i = 0; i < ARG_MAX; i++) {
        //    if(splitWords[i] != NULL) printf("%s\n", splitWords[i]);
        //    else break;
        //}
        //printf("%s\n", splitWords[0]);
        //printf("%s\n", splitWords[1]);
        //printf("%s\n", splitWords[2]);
        //printf("%s\n", splitWords[3]);
        //printf("%s\n", splitWords[4]);
        //printf("%s\n", splitWords[5]);
        //printf("%s\n", splitWords[6]);
    if (strcmp(splitWords[1], "DATABASE") == 0) {
        status = createDirectory(splitWords[2]);
        if(status == 0) printf("Database %s created.\n", splitWords[2]);
        else printf("!Failed to create database %s because it already exists.\n", splitWords[2]);
    }
    else if (strcmp(splitWords[1], "TABLE") == 0) {

        createTable(splitWords[2], splitWords, parameterCount);
    }
    else {
        printf("Error: Invalid syntax. Please try again.\n");
        return 1;
    }
    return 0;
}

int dropCommand(char *splitWords[]){
    int status = 1;
    if (strcmp(splitWords[1], "DATABASE") == 0) {
        
        status = rmdir(splitWords[2]);
        if(status == 0) printf("Database %s deleted.\n", splitWords[2]);
        else printf("!Failed to delete %s because it does not exist.\n", splitWords[2]);
    }
    else if (strcmp(splitWords[1], "TABLE") == 0) {
        printf("Deleting table %s\n", splitWords[2]);
        deleteTable(splitWords[2]);
    }
    else {
        printf("Error: Invalid syntax. Please try again.\n");
        return 1;
    }
    return 0;

}
int useCommand(char *splitWords[]){
    int status = 1;
    useCheck = 1;
    strcpy(workingPath, splitWords[1]);
    printf("Using database %s\n", splitWords[1]);
}
int selectCommand(char *splitWords[]){
    return -1;
}
int alterCommand(char *splitWords[]){
    return -1;
}

int createDirectory(const char* directoryName) {
    int check;
    check = mkdir(directoryName, 0777);
    return check;
}

int createTable(char* tableName, char* splitWords[], int parameterCount) {
    char filePath[100];
    char *tableParameters[parameterCount];
    strcpy(filePath, workingPath);
    strcat(filePath, "/");
    strcat(filePath, tableName);

    if (access(filePath, F_OK) == 0) {
        printf("File already exists\n");
        return 1;
    } else {
        FILE *fptr;
        fptr = fopen(filePath, "w");
        cleanTableParameters(splitWords, tableParameters, parameterCount);
        //printf("From createTable, printing tableParameters:\n");
        for(int i = 0; i < parameterCount; i++) {
            printf("%s\n", tableParameters[i]);
            fputs(tableParameters[i], fptr);
            //fputs(" ", fptr);
            if(i % 2 == 1) {
                if(i == parameterCount-1) fputs(",", fptr);
                else fputs(",\n", fptr);
            }
            else fputs(" ", fptr);
        }
        //fputs("Hello World", fptr);
        fclose(fptr);
        printf("Creating table %s\n", tableName);
        
        return 0;
    }
    //cleanTableParameters(splitWords, *tableParameters, parameterCount);
}

int deleteTable(const char* tableName){
    int status = 1;
    char filePath[100];
    strcpy(filePath, workingPath);
    strcat(filePath, "/");
    strcat(filePath, tableName);
    status = remove(filePath);
    return status;
}

int checkUse() {
    if(useCheck == 0) {
        printf("!Error: No database selected. Please use the USE command to select a database.\n");
        return 1;
    }
    else return 0;
}

void cleanTableParameters(char *splitWords[], char *tableParameters[], int parameterCount) {
    //printf("(In cleanTable) parameterCount: %d", parameterCount);
    //printf("Coming from cleanTableParameters\n");
    /*
    printf("Listing splitWords\n");
    for(int i = 0; i < ARG_MAX; i++) {
        if(splitWords[i] != NULL) printf("%d, %s\n", i, splitWords[i]);
        else break;
    }
    */
    // Copy splitWords into tableParameters
    for(int i = 0; i < parameterCount; i++) {
        //printf("iteration: %d\n", i);
        tableParameters[i] = splitWords[i+3];
    }
    //printf("tableParameters: \n");
    //for(int i = 0; i < parameterCount; i++) {
    //        if(tableParameters[i] != NULL) printf("%d, %s\n", i, tableParameters[i]);
    //        else break;
    //    }

    for(int i = 0; i < parameterCount; i++) {
        //printf("i: %d\n", i);
        if(i == 0) {
            if(strchr(tableParameters[i], '(') != NULL){
                removeChar(tableParameters[i], '(');
            }
        }
        if(i % 2 == 1) {
                if(strchr(tableParameters[i], ',') != NULL) removeChar(tableParameters[i], ',');
        }
        if(i == parameterCount-1) {
            if(strchr(tableParameters[i], ')') != NULL) tableParameters[i][strlen(tableParameters[i])-1] = '\0';
        }
    }
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}