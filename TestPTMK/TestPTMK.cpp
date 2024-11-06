#include <iostream>
#include <sqlite3.h>
#include "Employee.h"
#include <vector>
#include <chrono>

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

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		cerr << "Usage: TestPTMK <mode> [parameters]" << endl;
		return 1;
	}

	int mode = stoi(argv[1]);
	sqlite3* db;
	Employee handler("Smt", "2000-01-14", "male");
	
	bool rc = sqlite3_open("DataBase.db", &db);

	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		cerr << "Unable to open database:" << sqlite3_errmsg(db) << endl;
		return 1;
	}

	switch (mode) {
	case 1:
		createTable(db);
		break;
	case 2:
		if (argc < 5) {
			cerr << "Usage for mode 2: TestPTMK 2 <name> <birthDate> <sex>" << endl;
			return 1;
		}
		{
			string name = argv[2];
			string birthDate = argv[3];
			string sex = argv[4];
			Employee temp(name, birthDate, sex);
			temp.insert(db);
			cout << "Succesful" << endl;
		}
		break;
	case 3:
	{
		vector<Employee> employees;
		string sql = R"(
        SELECT e.name, e.birthDate, e.sex
        FROM Employees e
        GROUP BY e.name, e.birthDate
        ORDER BY e.name;
    )";
		employees = handler.pick(db, sql);
		for (auto emp : employees) {
			emp.printInfo();
		}
		break;
	}
	case 4:
	{
		vector<Employee> generatedEmployees = handler.generateEmployees(999900, false);
		vector<Employee> specialEmloyees = handler.generateEmployees(100, true);
		handler.insertMultiple(db, generatedEmployees);
		handler.insertMultiple(db, specialEmloyees);
		cout << "Succesful" << endl;
		break;
	}
	case 5:
	{
		auto start = chrono::high_resolution_clock::now();
		string sql = "SELECT name, birthDate, sex FROM Employees WHERE sex = 'male' AND name LIKE 'F%';";
		vector<Employee> employees = handler.pick(db, sql);
		for (auto emp : employees) {
			emp.printInfo();
		}
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<double> duration = end - start;
		cout << "execution time: " << duration.count() << "seconds" << endl;
		break;
	}
	default:
		cerr << "Invalid mode or parameters" << endl;
	}

	sqlite3_close(db);

	return 0;
}