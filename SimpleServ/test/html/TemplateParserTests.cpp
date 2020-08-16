#include "TemplateParserTests.h"
#include "html/TemplateParser.h"
#include "html/Model.h"

#include <cassert>
#include <iostream>

using namespace std;

void TestVariableReplacement()
{
  TemplateParser parser;
  Model::Data data = { { "key", "value" } };
  Model model(data);
  string docTemplate = "<html>data = {{ key }}</html>\n";
  string expected = "<html>data = value</html>\n";
  
  string result = parser.Apply(docTemplate, model);

  cout << "TestVariableReplacement: parser.Apply result [" << result << "]" << endl;
  
  assert(result == expected);
}

void TestConditionalTrue()
{
  TemplateParser parser;
  Model::Data data = { { "key", "value" } };
  Model model(data);
  string docTemplate = "{{ #key }}<html></html>{{ /key }}";
  string expected = "<html></html>";
  
  string result = parser.Apply(docTemplate, model);

  cout << "TestConditionalTrue: parser.Apply result [" << result << "]" << endl;
  
  assert(result == expected);
}

void TestConditionalFalse()
{
  TemplateParser parser;
  Model::Data data;
  Model model(data);
  string docTemplate = "{{ #key }}<html></html>{{ /key }}";
  string expected = "";
  
  string result = parser.Apply(docTemplate, model);

  cout << "TestConditionalFalse: parser.Apply result [" << result << "]" << endl;
  
  assert(result == expected);
}

void TemplateParserTests()
{
  cout << "TemplateParser tests" << endl;

  TestVariableReplacement();
  TestConditionalTrue();
  TestConditionalFalse();
}