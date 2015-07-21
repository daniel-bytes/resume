#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SimpleServ/HttpError.h"
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
			test += "GET /path/file.html HTTP/1.1\r\n";
			test += "From: person@test.com\r\n";
			test += "User-Agent: unittest / 1.0\r\n";
			test += "\r\n";
			test += "This is\r\n";
			test += "a test";

			try {
				HttpRequestMessage msg(test);

				Assert::AreEqual(std::string("GET"), msg.getMethod(), L"Method should be correct", LINE_INFO());
				Assert::AreEqual(std::string("/path/file.html"), msg.getPath(), L"Path should be correct", LINE_INFO());
				Assert::AreEqual(std::string("1.1"), msg.getHttpVersion(), L"HTTP Version should be correct", LINE_INFO());

				auto headers = msg.getHeaders();
				Assert::AreEqual(size_t(2), headers.size(), L"Headers count should be correct", LINE_INFO());
				Assert::IsTrue(headers.cend() != headers.find("From"), L"Headers should contain first key", LINE_INFO());
				Assert::IsTrue(headers.cend() != headers.find("User-Agent"), L"Headers should contain second key", LINE_INFO());
				Assert::AreEqual(std::string("person@test.com"), headers["From"], L"Headers should contain first value", LINE_INFO());
				Assert::AreEqual(std::string("unittest / 1.0"), headers["User-Agent"], L"Headers should contain second value", LINE_INFO());
				Assert::AreEqual(std::string("This is\na test"), msg.getBody(), L"Body should be correct", LINE_INFO());
			}
			catch (const HttpError &err) {
				std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
				std::wstring what = converter.from_bytes(err.what());
				what += converter.from_bytes(err.getAdditionalInfo());

				Assert::Fail(what.c_str());
			}
		}

	};
}