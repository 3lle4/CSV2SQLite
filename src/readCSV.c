// 
// 
// This file contains the implementation of CSV file parsing functions.
// It defines custom tokenisation and CSV parsing for reading CSV data.

#include "readCSV.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Custom tokenization function to split a string by a specified delimiter, 
 * but avoid splitting inside quotes.
 *
 * @param str the input string to tokenise
 * @param delimiter the character used as a delimiter
 * @return a ptr to the next token in the string, or NULL if no more tokens are found
 * 
 * @called_from: *readCSV()
 * @credit: https://stackoverflow.com/questions/9659697/parse-string-into-array-based-on-spaces-or-double-quotes-strings
 */
char *custom_strtok(char *str, char delimiter) {
    static char *token; // retain the value of the previous call
    if (str != NULL) // resets the token if a new string is passed
        token = str;

    if (token == NULL || *token == '\0') // return NULL if the string is empty
        return NULL;

    char *start = token;
    int inside_quotes = 0; // 0: false; !0: true

    while (*token != '\0') { // iterate through the string
        if (*token == '"') { // toggle the flag if a quote is encountered
            inside_quotes = !inside_quotes;
        }

        if (*token == delimiter && !inside_quotes) { // return a token if the delimiter is encountered and not inside quotes
            *token = '\0';
            token++;
            break;
        }

        token++;
    }

    return start;
}

// CSV parser function
CSVData *readCSV(char *filename, char separator) {
    // open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    // Initialise variables
    char line[4096]; // buffer
    int rowCount = 0;
    CSVData *csvData = (CSVData *)malloc(sizeof(CSVData)); // allocate memory for the struct
    if (csvData == NULL) {
        printf("Memory allocation failed (struct).\n");
        return NULL;
    }
    csvData->fields = NULL;
    csvData->rowCount = 0;
    csvData->colCount = NULL;

    // Read and process each line
    while (fgets(line, sizeof(line), file) != NULL) {
        char *token;
        int colCount = 0;
        char **fields = NULL;

        // Tokenize the line using custom separator function
        token = custom_strtok(line, separator);
        while (token != NULL) {
            // Allocate memory for the new field and copy the data (= token)
            fields = (char **)realloc(fields, (colCount + 1) * sizeof(char *));
            if (fields == NULL) {
                printf("Memory allocation failed (new fields array).\n");
                freeCSVData(csvData);
                return NULL;
            }

            fields[colCount] = (char *)malloc(strlen(token) + 1);  // memory for the string: string + 0 char
            if (fields[colCount] == NULL) {
                printf("Memory allocation failed (new field).\n");
            }
            strcpy(fields[colCount], token); //copy string in current field

            if (fields[colCount] == NULL) {
                printf("Memory allocation failed (copying data in field).\n");
                freeCSVData(csvData);
                return NULL;
            }
            token = custom_strtok(NULL, separator); // get the next token
            colCount++;
        }

        // Add the data to the CSV->fields array
        csvData->fields = (char ***)realloc(csvData->fields, (rowCount + 1) * sizeof(char **));  // add one row to 3D arr
        if (csvData->fields == NULL) {
            printf("Memory allocation failed (CSVData->fields).\n");
            freeCSVData(csvData);
            return NULL;
        }
        csvData->fields[rowCount] = fields; // adding new row of fields to the CSVData struct
        csvData->colCount = (int *)realloc(csvData->colCount, (rowCount + 1) * sizeof(int)); // more memory for colCount
        if (csvData->colCount == NULL) {
            printf("Memory allocation failed (colCount).\n");
            freeCSVData(csvData);
            return NULL;
        }
        csvData->colCount[rowCount] = colCount;
        rowCount++;
    }

    fclose(file); //close file when ready copying

    csvData->rowCount = rowCount; // saving final rowCount to the struct
    return csvData;
}

// function to free the allocated memory in CSVData
void freeCSVData(CSVData *data) {
    if (data != NULL) {
        for (int i = 0; i < data->rowCount; i++) {
            for (int j = 0; j < data->colCount[i]; j++) {
                free(data->fields[i][j]); // free individual fields
            }
            free(data->fields[i]); // free rows
        }
        free(data->fields); // free fields arr
        free(data->colCount);
        free(data); 
    }
}