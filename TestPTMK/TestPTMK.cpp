#include <iostream>
#include <sqlite3.h>
#include "Employee.h"
#include <vector>

using namespace std;

static bool createTable(sqlite3* db) {
	int rc;
	char* errMsg = 0;
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

static int callback(void* data, int argc, char** argv, char** colName) {
	auto* employees = static_cast<vector<Employee>*>(data);
	employees->emplace_back(argv[0] ? argv[0] : "", argv[1] ? argv[1] : "", argv[2] ? argv[2] : "");
	return 0;
}

static vector<Employee> getData(sqlite3* db) {
	char* errMsg = nullptr;
	vector<Employee> employees;
	string sql = "SELECT name, birthDate, sex FROM Employees";
	int rc = sqlite3_open("DataBase.db", &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
			return employees;
	}

	if (sqlite3_exec(db, sql.c_str(), callback, &employees, &errMsg) != SQLITE_OK) {
		cerr << "SQL error " << errMsg << endl;
		sqlite3_free(errMsg);
	}

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
	//Employee a1("GRISsa feef efef", "2005-15-01", "male");
	//a1.insert(db);
	
	vector<Employee> employees = getData(db);

	for (auto employee : employees) {
		employee.printInfo();
	}

	sqlite3_close(db);

	return 0;
}