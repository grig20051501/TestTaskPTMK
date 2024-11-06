#include <iostream>
#include <sqlite3.h>
#include "Employee.h"
#include <vector>

using namespace std;

static void createTable(sqlite3* db) {
	int rc;
	char* errMsg = 0;
	char sql1[] = "CREATE TABLE Employees(id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name TEXT, birthDate TEXT, sex TEXT);";

	rc = sqlite3_exec(db, sql1, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "Failed to create database: " << &errMsg << endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return;
	}
}

vector<Employee> readAllEmployees(sqlite3* db) {
    vector<Employee> employees;
    string sql = R"(
        SELECT e.name, e.birthDate, e.sex
        FROM Employees e
        GROUP BY e.name, e.birthDate
        ORDER BY e.name;
    )";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return employees;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* fullName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* birthDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* gender = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        if (fullName && birthDate && gender) {
            employees.emplace_back(fullName, birthDate, gender);
        }
        else {
            cerr << "Error: One or more columns contain NULL values!" << endl;
        }
    }

    sqlite3_finalize(stmt);
    return employees;
}

int main() {
	sqlite3* db;
	
	bool rc = sqlite3_open("DataBase.db", &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return 1;
	}

	//createTable(db);
	/*Employee a1("AAAA", "2005-15-01", "male");
    Employee a2("BBBB", "2006-15-01", "male");
    Employee a3("CCCC", "2007-15-01", "female");
    Employee a4("DDDD", "2008-15-01", "female");
	a1.insert(db);
    a2.insert(db);
    a3.insert(db);
    a4.insert(db);*/
	
	vector<Employee> employees = readAllEmployees(db);

	for (auto employee : employees) {
		employee.printInfo();
	}

	sqlite3_close(db);

	return 0;
}