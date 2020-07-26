#ifndef __HTTPERROR_H__
#define __HTTPERROR_H__

#include "Http.h"
#include "HttpResponseMessage.h"
#include <stdexcept>
#include <sstream>

class HttpError
	: public std::runtime_error
{
public:
	HttpError(Http::StatusCode statusCode, std::string additionalInfo)
		: std::runtime_error(Http::StatusDescriptions::Get(statusCode) + ": " + additionalInfo),
		_statusCode(statusCode),
		_additionalInfo(additionalInfo)
	{
	}
	
	~HttpError() throw() {}

	Http::StatusCode GetStatusCode() const {
		return _statusCode;
	}

	const std::string& GetAdditionalInfo() const {
		return _additionalInfo;
	}

	const HttpResponseMessage CreateResponse() const {
		return HttpResponseMessage(
				_statusCode,
				Http::ContentTypes::PlainText(),
				Http::DefaultHeaders,
				Http::StatusDescriptions::Get(_statusCode)
			);
	}

private:
	Http::StatusCode _statusCode;
	std::string _additionalInfo;
};

#endif //__HTTPERROR_H__