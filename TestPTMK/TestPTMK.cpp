#include <iostream>
#include <sqlite3.h>
#include "Employee.h"

using namespace std;

static bool createTable(sqlite3* db, char* errMsg) {
	int rc;
	char sql1[] = "CREATE TABLE Employees(id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT, birthDate TEXT, sex TEXT);";

	rc = sqlite3_exec(db, sql1, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "SQL ERROR: " << &errMsg << endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return 1;
	}

	return 0;
}

int main() {
	sqlite3* db;
	char *errMsg = 0;
	bool rc = sqlite3_open("DataBase.db", &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return 1;
	}

	createTable(db, errMsg);	
	sqlite3_close(db);

	return 0;
}