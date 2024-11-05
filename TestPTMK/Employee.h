#pragma once

#include <string>
#include <sqlite3.h>

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
	bool insert(sqlite3* db, char* errMsg);
};