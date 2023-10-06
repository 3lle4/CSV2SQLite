#include "readCSV.h"
#include "createDB.h"
#include "../sqlite/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    const char *filename = "../data/elements.csv";
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

    char *DBname = "test.db";
    int result = newDB("test.db");
    if (result == 1) {
        printf("Failed to create or open database.\n");
        return 1;
    }
    printf("Database created or opened successfully.\n");

    return 0;
}
