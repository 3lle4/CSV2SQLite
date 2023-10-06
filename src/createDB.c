#include "createDB.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int newDB(char *DBname) {
    char *path = malloc(strlen(DBname)+4); // create path for db folder
    sprintf(path, "../db/%s", DBname); //store formated string to buffer (path)

    sqlite3 *db; // declaration ptr to db
    char *err_msg = 0; //ptr to error msg
    
    int rc = sqlite3_open(path, &db); 
    /*
        Opens db or creates one with the name if it does not already exist. 
        Has an error code as return value which is stored in rc.
    */
    
    if (rc != SQLITE_OK) { // return error
        
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
}

// int insertData() {
//     char *sql = "DROP TABLE IF EXISTS Cars;" // SQL Statement
//                 "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);" 
//                 "INSERT INTO Cars VALUES(1, 'Audi', 52642);" 
//                 "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
//                 "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
//                 "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
//                 "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
//                 "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
//                 "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
//                 "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

//     rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
//     /*
//         Ausführung der SQL Statements (gespeichert in *sql); Speichern der Fehler in err_msg.
//     */
    
//     if (rc != SQLITE_OK ) { //gibt Fehler aus, falls Ausführung nicht erfolgreich
        
//         fprintf(stderr, "SQL error: %s\n", err_msg);
        
//         sqlite3_free(err_msg);        
//         sqlite3_close(db);
        
//         return 1;
//     } 
    
//     sqlite3_close(db); //db schließen
    
//     return 0;
// }

