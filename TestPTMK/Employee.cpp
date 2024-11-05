#include "Employee.h"
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

Employee::Employee(string name, string date, string sex) {
	this->name = name;
	this->birthDate = date;
	this->sex = sex == "male" ? "male" : "female";
}

void Employee::printInfo() {
	cout << "Name: " << this->name << endl;
	cout << "bithDate: " << this->birthDate << endl;
	cout << "sex: " << this->sex << endl;
	cout << endl;
}

int Employee::getAge() {
	return 2024 - stoi(this->birthDate.substr(0, 4));
}

bool Employee::insert(sqlite3* db) {
	char* errMsg = nullptr;
	string sql = "INSERT INTO Employees (name, birthDate, sex) VALUES"
				"('" + this->name +
				"', '" + this->birthDate +
				"', '" + this->sex + "')";
	
	int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "SQL ERROR: " << errMsg << endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return 1;
	}

	return 0;
}