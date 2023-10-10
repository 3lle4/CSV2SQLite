//
//
// Header-file for the DB_handling module. 
// The module holds functions to create or open SQLite database files and make new tables.
// It takes CSVData objects and stores them in the db file and table.
// + Functions to work with the Database.

#ifndef DBHANDLING_H // include guards to ensure that the header file is only compiled once
#define DBHANDLING_H

#include "readCSV.h"

/**
 * Function to create new DB/open an existing DB.
 *
 * @param DBname name of the SQLite database to create or open
 * @return 0 on success, -1 on failure
 */
int newDB(char *DBname);

/**
 * Function to create a new table in the specified SQLite database and fill it with data from CSVData.
 *
 * @param dbPath path to the SQLite database file
 * @param csvData a pointer to the CSVData object containing the data to be stored in the table
 * @param tableName name of the table to create
 * @return 0 on success, -1 on failure
 */
int createTable(char *dbPath, CSVData *csvData, char *tableName);

/**
 * Function to insert data from CSVData into an existing table in the specified SQLite database.
 *
 * @param dbPath path to the SQLite database file
 * @param csvData a pointer to the CSVData object containing the data to be inserted: first row is turned to column titles
 * @param tableName  name of the table where data will be inserted
 * @return 0 on success, -1 on failure.
 */
int insertData(char *dbPath, CSVData *csvData, char *tableName);


/**
 * Search for a value in a specific column of a table in an SQLite database.
 *
 * This function performs a case-insensitive search for a given (text) value in a specified
 * column of a table in the SQLite database located at 'dbPath'. It constructs and
 * executes an SQL SELECT statement and prints the matching results.
 *
 * @param dbPath path to the SQLite database file
 * @param tableName name of the table to search within
 * @param columnName name of the column to search in
 * @param searchValue the value to search for (case-insensitive)
 *
 * @return 0 on success, 1 on failure.
 */
int searchInColumn(char *dbPath, char *tableName, char *columnName, char *searchValue);

/**
 * Print the column names of a table of an SQLite database.
 *
 * This function connects to the SQLite database by 'dbPath', gets
 * the column names and prints them.
 *
 * @param dbPath path to the SQLite database file
 * @param tableName the name of the table from which to retrieve column names
 */
void printColumnNames(char *dbPath, char *tableName);

/**
 * Cast the data type of a column in an SQLite table.
 *
 * This function connects to a SQLite database and updates
 * the data type of the specified column in a table to the target data type. 
 * 
 * @param dbPath  path to the SQLite database
 * @param tableName the name of the table containing the column to be cast
 * @param columnName name of the column to be cast
 * @param targetDataType target data type to which the column should be cast (e.g., "INTEGER")
 * 
 * @return 0 on success, 1 on failure.
 */
int castColumnDataType(char *dbPath, char *tableName, char *columnName, char targetDataType);

#endif
