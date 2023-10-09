 // 
 // Header-file for the readCSV module. 
 // The module handles csv files with customisable separators, whose lines are separated with newline.
 // The parsed csv is stored in a struct for further processing.

#ifndef READCSV_H // include guards to ensure that the header file is only compiled once
#define READCSV_H

// struct to store CSV data
typedef struct {
    char ***fields; // 3D array for pointer to fields array, ptr to each row, ptr to each string in the fields
    int rowCount;
    int *colCount;
} CSVData;

// Function to read CSV file and store it in the struct; returns ptr to CSVData object
CSVData *readCSV(char *filename, char separator);

// Function to free the allocated memory in CSVData
void freeCSVData(CSVData *data);

#endif