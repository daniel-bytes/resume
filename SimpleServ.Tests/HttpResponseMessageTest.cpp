#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SimpleServ/HttpResponseMessage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleServTests
{
	TEST_CLASS(HttpResponseMessageTest)
	{
	public:

		TEST_METHOD(HttpResponseMessage_Test_ValidMessage)
		{
			std::string test;
			test += "HTTP/1.1 200 OK\n";
			test += "Content-Type: text/plain\n";
			test += "Content-Length: 14\n";
			test += "Header1: Value\n";
			test += "\n";
			test += "This is\n";
			test += "a test";

			Http::Headers headers = {
				{ "Header1", "Value" }
			};
			HttpResponseMessage msg(Http::StatusCode_OK, "text/plain", headers, "This is\na test");

			Assert::AreEqual(test, msg.GetBytes(), L"Binary results should be correct", LINE_INFO());
		}
	};
}