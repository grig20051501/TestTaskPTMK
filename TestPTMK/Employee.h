#pragma once

#include <string>

using namespace std;

class Employee
{
private:
	string name;
	string birthDate;
	bool isMale;

public:
	void printInfo();
	int getAge();
	Employee(string name, string date, bool isMale);
};