#include "shared/Configuration.h"
#include "test/html/TemplateParserTests.h"
#include "test/http/HttpRequestMessageTests.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	cout << "*** Initializing test framework" << endl;
	Configuration config(argc, argv);
	
	cout << "*** Executing tests" << endl;
	TemplateParserTests();
	HttpRequestMessageTests();

	cout << "*** Test execution complete" << endl;
	return 0;
}