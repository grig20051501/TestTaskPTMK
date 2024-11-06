#pragma once

#include <string>
#include <sqlite3.h>
#include <vector>

using namespace std;

class Employee
{
public:
	string name;
	string birthDate;
	string sex;

	void printInfo();
	int getAge();
	Employee(string name, string date, string sex);
	void insert(sqlite3* db);
	void insertMultiple(sqlite3* db, vector<Employee> employees);
	vector<Employee> generateEmployees(size_t count, bool specialCase = false);
	vector<Employee> pick(sqlite3* db, string sql);
};