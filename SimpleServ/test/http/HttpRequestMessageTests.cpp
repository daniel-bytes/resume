#include "HttpRequestMessageTests.h"
#include "http/HttpRequestMessage.h"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

void TestEmptyMessage()
{
  HttpRequestMessage message("", "", 1234);

  assert(message.GetPort() == 1234);
  assert(message.GetHost() == "");
  assert(message.GetBody() == "");
  assert(message.GetHttpVersion() == "");
  assert(message.GetIpAddress() == "");
  assert(message.GetMethod() == "");
  assert(message.GetPath() == "");
  assert(message.GetHeaders().size() == 0);
  assert(message.GetQuery().size() == 0);
  assert(message.GetRequestId() != ""); // auto-generated
}

void TestNonEmptyMessage()
{
  ostringstream oss;
  oss << "POST /foo/bar?key=value&other=123 HTTP/1.1\n"
      << "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
      << "Host: www.test.com\n"
      << "X-Forwarded-For: 6.7.8.9\n"
      << "X-Request-Id: 12345\n"
      << "\n"
      << "<html>\n"
      << "test\n"
      << "</html>";

  HttpRequestMessage message(oss.str(), "1.2.3.4", 1234);

  assert(message.GetPort() == 1234);
  assert(message.GetHost() == "www.test.com");
  assert(message.GetBody() == "<html>\ntest\n</html>");
  assert(message.GetHttpVersion() == "1.1");
  assert(message.GetIpAddress() == "6.7.8.9");
  assert(message.GetMethod() == "POST");
  assert(message.GetPath() == "/foo/bar");
  assert(message.GetHeaders().size() == 4);
  assert(message.GetHeaders().at("User-Agent") == "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)");
  assert(message.GetHeaders().at("Host") == "www.test.com");
  assert(message.GetHeaders().at("X-Forwarded-For") == "6.7.8.9");
  assert(message.GetHeaders().at("X-Request-Id") == "12345");
  assert(message.GetQuery().size() == 2);
  assert(message.GetQuery().at("key") == "value");
  assert(message.GetQuery().at("other") == "123");
  assert(message.GetRequestId() == "12345");
  
}

void HttpRequestMessageTests()
{
  cout << "*** HttpRequestMessage tests" << endl;

  TestEmptyMessage();
  TestNonEmptyMessage();
}