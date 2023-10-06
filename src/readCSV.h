// struct to store CSV data
typedef struct CSVData {
    char ***fields;
    int rowCount;
    int *colCount;
} CSVData;

// Function to read CSV file and store it in the struct
CSVData *readCSV(const char *filename, char separator);

// Function to free the allocated memory in CSVData
void freeCSVData(CSVData *data);