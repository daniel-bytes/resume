#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SimpleServ/HttpRequestMessage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimpleServTests
{		
	TEST_CLASS(HttpRequestMessageTest)
	{
	public:
		
		TEST_METHOD(HttpRequestMessage_Test_ValidMessage)
		{
			std::string test;
			test += "GET /path/file.html HTTP/1.0\n";
			test += "From: person@test.com\n";
			test += "User-Agent: unittest / 1.0\n";
			test += "\n";
			test += "This is\n";
			test += "a test";

			HttpRequestMessage msg(test);

			Assert::AreEqual(std::string("GET"), msg.getMethod(), L"Method should be correct", LINE_INFO());
			Assert::AreEqual(size_t(2), msg.getHeaders().size(), L"Headers count should be correct", LINE_INFO());

			auto headers = msg.getHeaders();
			Assert::IsTrue(headers.cend() != headers.find("From"), L"Headers should contain first key", LINE_INFO());
			Assert::IsTrue(headers.cend() != headers.find("User-Agent"), L"Headers should contain second key", LINE_INFO());
			Assert::AreEqual(std::string("person@test.com"), headers["From"], L"Headers should contain first value", LINE_INFO());
			Assert::AreEqual(std::string("unittest / 1.0"), headers["User-Agent"], L"Headers should contain second value", LINE_INFO());
			Assert::AreEqual(std::string("This is\na test"), msg.getBody(), L"Body should be correct", LINE_INFO());
		}

	};
}