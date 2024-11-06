#include <iostream>
#include <sqlite3.h>
#include "Employee.h"
#include <vector>
#include <random>

//R"(
//        SELECT e.name, e.birthDate, e.sex
//        FROM Employees e
//        GROUP BY e.name, e.birthDate
//        ORDER BY e.name;
//    )"

//"SELECT*"
//"FROM Employees"
//"WHERE gender = 'Male' AND name LIKE 'F%';"

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

int main() {
	sqlite3* db;
	string sql = "SELECT name, birthDate, sex FROM Employees WHERE sex = 'male' AND name LIKE 'F%';";

	Employee a1("Smt", "2000-01-14", "male");
	
	bool rc = sqlite3_open("DataBase.db", &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return 1;
	}

	/*vector<Employee> employessGenerated = a1.generateEmployees(990000);
	vector<Employee> speciaEmployees = a1.generateEmployees(100, true);*/
	vector<Employee> employees = a1.pick(db, sql);
	for (auto i : employees) {
		i.printInfo();
	}

	/*a1.insertMultiple(db, employessGenerated);
	a1.insertMultiple(db, speciaEmployees);*/

	sqlite3_close(db);

	return 0;
}