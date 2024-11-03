#include <iostream>
#include "Employee.h"

using namespace std;

int main() {
	Employee a1("grisha", "2005-01-15", true);
	cout << "Hello" << endl;

	a1.printInfo();
	cout << a1.getAge();

	return 0;
}