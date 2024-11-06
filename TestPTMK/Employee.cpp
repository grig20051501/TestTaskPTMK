#include "Employee.h"
#include <iostream>
#include <algorithm>
#include <sqlite3.h>
#include <random>
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

void Employee::insertMultiple(sqlite3* db, vector<Employee> employees) {
	string sql = "BEGIN TRANSACTION;";
	for (const auto& emp : employees) {
		sql += "INSERT INTO Employees (name, birthDate, sex) VALUES ('" +
			emp.name + "', '" + emp.birthDate + "', '" + emp.sex + "');";
	}
	sql += "COMMIT;";

	char* errMsg = nullptr;
	if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
		cerr << "Error inserting data: " << errMsg << endl;
		sqlite3_free(errMsg);
	}
}

vector<Employee> Employee::generateEmployees(size_t count, bool specialCase) {
	vector<Employee> employees;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> genderDist(0, 1);
	uniform_int_distribution<> letterDist(0, 25);
	uniform_int_distribution<> dayDist(1, 28);
	uniform_int_distribution<> monthDist(1, 12);
	uniform_int_distribution<> yearDist(1970, 2000);

	for (size_t i = 0; i < count; ++i) {
		string sex;
		if ((genderDist(gen) || specialCase)) {
			sex = "male";
		}
		else {
			sex = "female";
		}
		char firstLetter = 'A' + letterDist(gen);
		string lastName = (specialCase) ? "F" + string(1, 'A' + letterDist(gen)) + "lastname" : string(1, firstLetter) + "lastname";
		string firstName = "Firstname";
		string birthDate = to_string(yearDist(gen)) + "-" +
			(monthDist(gen) < 10 ? "0" : "") + to_string(monthDist(gen)) + "-" +
			(dayDist(gen) < 10 ? "0" : "") + to_string(dayDist(gen));

		employees.emplace_back(lastName + " " + firstName, birthDate, sex);
	}

	return employees;
}

vector<Employee> Employee::pick(sqlite3* db, string sql) {
	vector<Employee> employees;
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
		return employees;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const char* fullName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		const char* birthDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		const char* sex = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

		if (fullName && birthDate && sex) {
			employees.emplace_back(fullName, birthDate, sex);
		}
		else {
			cerr << "Error: One or more columns contain NULL values!" << endl;
		}
	}

	sqlite3_finalize(stmt);
	return employees;
}