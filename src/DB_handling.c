// 
// 
// This file contains the implementation of DB_handling functions.
// It defines creating a SQLite database as well as table and fill it with data from a CSVData struct.
// 

#include "DB_handling.h"
#include "readCSV.h"
#include "../sqlite/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function to create new db
int newDB(char *db_name) {
    char *path = malloc(strlen(db_name)+4); // create path from given name to db folder
    sprintf(path, "../db/%s", db_name); //store formated string to buffer (path)

    sqlite3 *db; // declaration ptr to db
    
    int rc = sqlite3_open(path, &db); // Opens db or creates one with the name if it does not already exist. Has an error code as return value which is stored in rc.
    
    if (rc != SQLITE_OK) { // return error if opening fails
        
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        
        return 1;
    }

    free(path);
    sqlite3_close(db);
    return 0;
}

// function to create a new table in an existing db
int createTable(char *dbPath, CSVData *csvData, char *tableName) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s.\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Build the CREATE TABLE SQL statement based on the first row of CSVData
    char *sql = malloc(4096);
    if (sql == NULL) {
        printf("Memory allocation failed (creating the table).\n");
        sqlite3_close(db);
        return 1;
    }

    // build SQL statement: 
    // CREATE TABLE IF NOT EXISTS tablename (col1, col2, ..., coln TEXT);
    strcpy(sql, "CREATE TABLE IF NOT EXISTS "); // copy the parts in the sql stmt
    strcat(sql, tableName);
    strcat(sql, " (");

    for (int n = 0; n < csvData->colCount[0]; ++n) { // iterate through first row
        if (n > 0) {
            strcat(sql, ", "); // add column names to SQL stmt, separated by ,
        }

        // Remove double quotes from the column name if they exist to prevent empty col names
        char *cleanedColName = csvData->fields[0][n];
        if (cleanedColName[0] == '"' && cleanedColName[strlen(cleanedColName) - 1] == '"') {
            cleanedColName[strlen(cleanedColName) - 1] = '\0';
            cleanedColName = &cleanedColName[1];
        }

        strcat(sql, cleanedColName); // put in col names in sql statement
        strcat(sql, " TEXT"); // assuming all columns are of type TEXT
    }

    strcat(sql, ");");

    // Execute the SQL statement to create the table
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg); // 0, 0: params of callback function, not needed here

    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n.", errMsg); // print error if one occurs
        sqlite3_free(errMsg); 
        free(sql);
        sqlite3_close(db);
        return 1;
    }

    free(sql);
    sqlite3_close(db);
    return 0;
}


/**
 * Replace single quotes with doubled single quotes in the input string.
 *
 * This function takes an input string and returns a new string where all single
 * quotes (') are replaced with two consecutive single quotes (''). 
 * Needed to prevent input errors.
 *
 * @param input The input string in which single quotes will be replaced.
 *
 * @return a string with single quotes replaced by doubled
 * single quotes, or NULL if allocation fails. 
 * 
 * Remember freeing the returned string!
 * 
 * @called_from: insertData()
 */
char *replaceSingleQuotes(char *input) { // neccessary bc SQLite uses single quotes as string marker
    int len = strlen(input); 
    char *result = malloc(2 * len + 1);

    if (result == NULL) {
        return NULL;
    }

    int n = 0;

    for (int i = 0; i < len; i++) { // iterate through chars of str and replace ' with ''
        if (input[i] == '\'') {
            result[n++] = '\''; // add in two steps
            result[n++] = '\'';
        } else {
            result[n++] = input[i];
        }
    }

    result[n] = '\0'; // add null char at the end
    return result;
}


// Function to insert data from CSVData into an SQLite database
int insertData(char *dbPath, CSVData *csvData, char *tableName) {
    char *errMsg = 0;
    int rc;

    sqlite3 *db;
    rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Loop through each row in the CSVData struct beginning with the second (assuming first one holds the col names)
    for (int i = 1; i < csvData->rowCount; i++) {
        // space for size of line
        int line_size = 0;

        // Iterate through the columns of the row
        for (int n = 0; n < csvData->colCount[i]; n++) {
            // Add the length of the field (text) and account for additional characters
            line_size += strlen(csvData->fields[i][n])+3; //for quotes etc.
        }

        line_size += 100;  // add safety margin

        // Build the INSERT SQL statement 
        char *sql = malloc(line_size);
        if (sql == NULL) {
            printf("Memory allocation failed (inserting data in table)\n");
            sqlite3_close(db);
            return 1;
        }
        
        // build SQL statement: 
        // INSERT INTO tablename VALUES ('value1', 'value2', ..., 'vanluen'); // treating all values as text
        strcpy(sql, "INSERT INTO ");
        strcat(sql, tableName);
        strcat(sql, " VALUES (");

        // Loop through each column in the row
        for (int n = 0; n < csvData->colCount[i]; n++) {
            if (n > 0) {
                strcat(sql, ", ");
            }
            
            // escape single quotes by doubling them up
            char *escapedValue = replaceSingleQuotes(csvData->fields[i][n]); // check every field for single quotes
            
            if (strlen(escapedValue) == 0) {
                strcat(sql, "NULL"); // Insert NULL for empty values
            } else {
                strcat(sql, "'");
                strcat(sql, escapedValue);
                strcat(sql, "'");
            }
            
            free(escapedValue); // free the string from replaceSingleQuotes()
        }

        strcat(sql, ");");

        // Execute the SQL statement
        rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

        if (rc != SQLITE_OK) {
            printf("SQL error: %s\n", errMsg);
            sqlite3_free(errMsg);
            free(sql);
            sqlite3_close(db);
            return 1;
        }

        free(sql);
    }

    sqlite3_close(db);
    return 0;
}


/**
 * Print the results of an SQL query.
 *
 * This function is used to print the results of an SQL query. 
 * It iterates through the columns and prints both column names
 * and their corresponding values for each row in the result set.
 *
 * @param data input data
 * @param columns The number of columns in the set
 * @param columnValues array of column values for the current row
 * @param columnNames an array of column names
 *
 * @called_from searchInColumn()
 */
int printResults(void *data, int columns, char **columnValues, char **columnNames) {
    printf("Search results:\n");
    for (int i = 0; i < columns; i++) {
        printf("%s: %s\n", columnNames[i], columnValues[i]);
    }
    printf("\n");

    return 0;
}

// Function to search for a value in a specific column of a table
int searchInColumn(char *dbPath, char *tableName, char *columnName, char *searchValue) {
    sqlite3 *db;
    int rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // build the SQL SELECT statement
    char *sql = sqlite3_mprintf("SELECT * FROM %q WHERE LOWER(%q) LIKE LOWER('%%%q%%');", tableName, columnName, searchValue); // case insensitive

    char *errMsg = NULL;

    rc = sqlite3_exec(db, sql, printResults, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);

    return 0;
}

// Function to print column names of an SQLite table
void printColumnNames( char *dbPath, char *tableName) {
    sqlite3 *db;
    int rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char query[100];
    snprintf(query, sizeof(query), "PRAGMA table_info(%s);", tableName); //write formatted output to sized buffer (query)

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0); // compile the sql statement; last param is not needed

    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) { // iterate through the col names in the stmt and print them
        char *colName = (char *)sqlite3_column_text(stmt, 1);
        if (colName==NULL){
            continue;
        } else {
        printf("%s\n", colName);
        }
    }

    sqlite3_finalize(stmt); // destroy the stmt object
    sqlite3_close(db); // close db
}

//function to cast a column's datatype
int castColumnDataType(char *dbPath, char *tableName, char *columnName, char targetDataType) {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = NULL;

    // generate SQL based on the new datatype
    switch (targetDataType) {
        case 'I': // INTEGER
            asprintf(&sql, "UPDATE %s SET %s = CAST(%s AS INTEGER);", tableName, columnName, columnName); // asprintf holds pointer to formatted output (sprintf to small)
            break;
        case 'R': // REAL
            asprintf(&sql, "UPDATE %s SET %s = CAST(%s AS REAL);", tableName, columnName, columnName);
            break;
        case 'T': // TEXT
            asprintf(&sql, "UPDATE %s SET %s = CAST(%s AS TEXT);", tableName, columnName, columnName);
            break;
        case 'B': // BLOB
            asprintf(&sql, "UPDATE %s SET %s = CAST(%s AS BLOB);", tableName, columnName, columnName);
            break;
        default:
            printf("Unsupported data type: %c\n", targetDataType);
            sqlite3_close(db);
            return 1;
    }

    if (sql == NULL) {
        printf("Memory allocation failed\n");
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, sql, 0, 0, 0);

    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", sqlite3_errmsg(db));
        free(sql);
        sqlite3_close(db);
        return 1;
    }

    free(sql);
    sqlite3_close(db);
    return 0;
}
