#include "Employee.h"
#include <iostream>
#include <string>

using namespace std;

Employee::Employee(string name, string date, bool isMale) {
	this->name = name;
	this->birthDate = date;
	this->isMale = isMale;
}

void Employee::printInfo() {
	cout << "Name: " << this->name;
	cout << " bithDate: " << this->birthDate;
	string sex = this->isMale ? "male" : "female";
	cout << " sex: " << sex << endl;
}

int Employee::getAge() {
	return 2024 - stoi(this->birthDate.substr(0, 4));
}