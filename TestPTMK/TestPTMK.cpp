#include <iostream>
#include <sqlite3.h>
#include "Employee.h"

using namespace std;

int main() {
	sqlite3* DB;
	bool ex = false;
	sqlite3_open("DataBase.db", &DB);

	if (ex) { cout << "Error"; }
	else { cout << "Succesfull"; }
	Employee a1("grisha", "2005-01-15", true);
	cout << "Hello" << endl;

	a1.printInfo();
	cout << a1.getAge();

	sqlite3_close(DB);

	return 0;
}