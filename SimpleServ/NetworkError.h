#ifndef __NETWORKERROR_H__
#define __NETWORKERROR_H__

#include <stdexcept>
#include <sstream>

class NetworkError
	: public std::runtime_error
{
public:
	NetworkError(const std::string &msg, int errorCode)
		: std::runtime_error(msg), _errorCode(errorCode)
	{
	}

	int getErrorCode() const {
		return _errorCode;
	}

public:
	static NetworkError Create(const std::string &msg, int errorCode)
	{
		std::ostringstream oss;
		oss << msg << "  Error Code " << errorCode;
		
		auto str = oss.str();

		return NetworkError(str, errorCode);
	}

private:
	int _errorCode;
};

#endif //__NETWORKERROR_H__