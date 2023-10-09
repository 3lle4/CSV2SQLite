#include "readCSV.h"
#include "DB_handling.h"
#include "../sqlite/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    // initialise variables
    char *db_name = NULL; // database name
    char *tb_name = NULL; // table name
    char *csv_path = NULL; // path to CSV
    char sep = ','; // custom separator
    char *db_path = (char*)(malloc(100)); // memory for db_path; build from db_name
    int res_db = 1; // result of creating db
    int res_tb = 1; // result of creating table
    CSVData *csv_struct = NULL; // struct for the data
    char col_name[50]; // name of col to search
    char value[250]; // value to search for
    
    // get parameters for the database, table and CSV
    printf("Hello!\nDo you want to create a new database?\n[y/n]\n");
    char ans_db = 'n';
    scanf("%s", &ans_db);
    if (ans_db == 'y') {
        db_name = (char*)malloc(50);
        printf("Name your database <example.db>: ");
        scanf(" %s", db_name);
    }

    printf("\nDo you want to create a new table?\n[y/n]\n");
    char ans_tb = 'n';
    scanf(" %s", &ans_tb);
    if (ans_tb == 'y') {
        tb_name = (char*)malloc(50);
        printf("Name your table <example>: ");
        scanf(" %s", tb_name);

        csv_path = (char*)malloc(100);
        printf("\nType in the path to your CSV <../data/example.csv>: ");
        scanf(" %s", csv_path);

        printf("\nType your separator <,>: ");
        getchar(); // consume nl
        scanf("%c", &sep);
    }

    sprintf(db_path, "../db/%s", db_name); // build path from db_name

    // inserting the data
    if (ans_tb == 'y') {
        if (ans_db == 'y') {
            printf("\nCreating table in new database and inserting CSV...\n");
            res_db = newDB(db_name);
        } else if (ans_db == 'n') {
            printf("\nCreating new table from CSV in existing database...\n");
        } else {
            printf("\nPlease input y or n!\n");
            return 1;
        }
        
        if (csv_path != NULL) {
            csv_struct = readCSV(csv_path, sep);
            res_tb = createTable(db_path, csv_struct, tb_name);

            if (res_tb == 0) {
                printf("Table created successfully!\n");
                
                printf("Inserting the data...\n");
                int res_insert = insertData(db_path, csv_struct, tb_name);
                if (res_insert == 0) {
                    printf("Data inserted successfully!\n");
                } else {
                    printf("Error when inserting data.\n");
                    return 1;
                }
            } else {
                printf("Error creating the table.\n");
            }
        } else {
            printf("Type in a valid path for your CSV!\n");
        }
        free(csv_path);
    }

    if (res_db == 0 && res_tb == 0) {
        printf("Successfully opend the database and created the table!\n");
    }
    
    // demo of casting a column's datatype
    int res_cast = castColumnDataType(db_path, tb_name, "year", 'I');
    
    if (res_cast == 0) {
        printf("Column %s in table %s successfully cast to %s\n", "year", tb_name, "INTEGER");
    } else {
        printf("Failed to cast column %s to %s\n", "year", "INTEGER");
    }

    printf("\nNow lets work with the database:\nIn which table do you want to search? ");
    scanf(" %s", tb_name);
    printf("\nHere are the columns of your table: ");
    printColumnNames(db_path, tb_name);
    getchar(); // consume nl

    while (1) { // loop for data retrieval
        printf("\nType the column you want to search (or type 'exit' to quit): ");
        scanf(" %s", col_name);

        if (strcmp(col_name, "exit") == 0) {
            break; // Exit the loop and end the program if the user types 'exit'
        }

        printf("\nType the (string) value you want to search for: ");
        scanf(" %s", value);

        if (searchInColumn(db_path, tb_name, col_name, value) != 0) {
            printf("Search failed.\n");
        } else {
            printf("Search successful.\n");
        }
    }

    // Clean up
    free(db_name);
    free(tb_name);
    free(db_path);

    return 0;
}