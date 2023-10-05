#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to store CSV data
typedef struct CSVData {
    char ***fields;
    int rowCount;
    int *colCount;
} CSVData;

void freeCSVData(CSVData *data);

// Custom tokenization function: prevents splitting inside quotes
char *custom_strtok(char *str, char delimiter) {
    static char *token; // retain the value of the previous call
    if (str != NULL) // resets the token if a new string is passed
        token = str;

    if (token == NULL || *token == '\0') // return NULL if the string is empty
        return NULL;

    char *start = token;
    int inside_quotes = 0;

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

// function to read CSV file and store it in the struct
CSVData *readCSV(const char *filename, char separator) {
    // open file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file");
        return NULL;
    }

    // Initialize variables
    char line[4096]; // buffer
    int rowCount = 0;
    CSVData *csvData = (CSVData *)malloc(sizeof(CSVData)); // allocate memory for the struct
    if (csvData == NULL) {
        printf("Memory allocation failed");
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
                printf("Memory allocation failed");
                freeCSVData(csvData);
                return NULL;
            }
            fields[colCount] = strdup(token); //duplicate the string and allocate memory for it
            if (fields[colCount] == NULL) {
                printf("Memory allocation failed");
                freeCSVData(csvData);
                return NULL;
            }
            token = custom_strtok(NULL, separator); // get the next token
            colCount++;
        }

        // Add the record to the array
        csvData->fields = (char ***)realloc(csvData->fields, (rowCount + 1) * sizeof(char **));
        if (csvData->fields == NULL) {
            printf("Memory allocation failed");
            freeCSVData(csvData);
            return NULL;
        }
        csvData->fields[rowCount] = fields;
        csvData->colCount = (int *)realloc(csvData->colCount, (rowCount + 1) * sizeof(int));
        if (csvData->colCount == NULL) {
            printf("Memory allocation failed");
            freeCSVData(csvData);
            return NULL;
        }
        csvData->colCount[rowCount] = colCount;
        rowCount++;
    }

    fclose(file);

    csvData->rowCount = rowCount;
    return csvData;
}

// Function to free the allocated memory in CSVData
void freeCSVData(CSVData *data) {
    if (data != NULL) {
        for (int i = 0; i < data->rowCount; i++) {
            for (int j = 0; j < data->colCount[i]; j++) {
                free(data->fields[i][j]);
            }
            free(data->fields[i]);
        }
        free(data->fields);
        free(data->colCount);
        free(data);
    }
}

int main() {
    const char *filename = "lib/elements.csv";
    char separator = ',';

    CSVData *data = readCSV(filename, separator);

    if (data != NULL) {
        // for (int i = 0; i < data->rowCount; i++) {
        //     printf("Row %d:\n", i + 1);
        //     for (int j = 0; j < data->colCount[i]; j++) {
        //         printf("Field %d: %s\n", j + 1, data->fields[i][j]);
        //     }
        // }
        printf("Total rows: %d\n", data->rowCount);
        printf("%s\n", data->fields[0][0]);

        // Free allocated memory when done
        freeCSVData(data);
    }

    return 0;
}

