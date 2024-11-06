#include "Employee.h"
#include <iostream>
#include <algorithm>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

Employee::Employee(string name, string date, string sex) {
	this->name = name;
	this->birthDate = date;
	this->sex = sex == "male" ? "male" : "female";
}

int Employee::getAge() {
	string year = this->birthDate.substr(0, 4);
	if (all_of(year.begin(), year.end(), isdigit)) {
		return 2024 - stoi(year);
	}
	else return -1;
}

void Employee::printInfo() {
	cout << "Name: " << this->name << endl;
	cout << "bithDate: " << this->birthDate << endl;
	cout << "sex: " << this->sex << endl;
	cout << "age: " << this->getAge() << endl;
	cout << endl;
}

void Employee::insert(sqlite3* db) {
	char* errMsg = nullptr;
	string sql = "INSERT INTO Employees (name, birthDate, sex) VALUES"
				"('" + this->name +
				"', '" + this->birthDate +
				"', '" + this->sex + "')";
	
	int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		cout << "Failed to insert data: " << errMsg << endl;
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return;
	}
}