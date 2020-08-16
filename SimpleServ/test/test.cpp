#include "app/Configuration.h"
#include "test/html/TemplateParserTests.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	cout << "Initializing test framework" << endl;
	Configuration::Initialize(argc, argv);
	
	cout << "Executing tests" << endl;
	TemplateParserTests();

	cout << "Test execution complete" << endl;
	return 0;
}