#ifndef __HTTPERROR_H__
#define __HTTPERROR_H__

#include "Http.h"
#include "HttpResponseMessage.h"
#include <stdexcept>
#include <sstream>

/**
 * An error that is thrown when an HTTP operation fails
 */
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

  /** Returns the HTTP status code of the error **/
	Http::StatusCode GetStatusCode() const { return _statusCode; }

  /** Returns additional text info of the error **/
	const std::string& GetAdditionalInfo() const { return _additionalInfo; }

  /**
	 * Converts the HttpError to an HttpResponseMessage
	 */
	const HttpResponseMessage CreateResponse() const {
		return HttpResponseMessage(
				_statusCode,
				Http::ContentTypes::PlainText(),
				{},
				Http::StatusDescriptions::Get(_statusCode),
				HttpRequestMessage::GenerateRequestId()
			);
	}

private:
	Http::StatusCode _statusCode;
	std::string _additionalInfo;
};

#endif //__HTTPERROR_H__