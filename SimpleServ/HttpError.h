#ifndef __HTTPERROR_H__
#define __HTTPERROR_H__

#include <stdexcept>
#include <sstream>
#include "HttpStatus.h"

class HttpError
	: public std::runtime_error
{
public:
	HttpError(Http::StatusCode statusCode, std::string additionalInfo)
		: std::runtime_error(Http::StatusDescriptions::Get(statusCode)),
		_statusCode(statusCode),
		_additionalInfo(additionalInfo)
	{
	}
	
	~HttpError(void) throw() {}

	int getStatusCode() const {
		return _statusCode;
	}

	const std::string& getAdditionalInfo() const {
		return _additionalInfo;
	}

private:
	Http::StatusCode _statusCode;
	std::string _additionalInfo;
};

#endif //__HTTPERROR_H__