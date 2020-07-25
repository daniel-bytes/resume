#ifndef __NETWORKERROR_H__
#define __NETWORKERROR_H__

#include <errno.h>
#include <stdexcept>
#include <sstream>
#include <cstring>

class TcpError
	: public std::runtime_error
{
public:
	TcpError(const std::string &msg)
		: TcpError(msg, errno) {}

	TcpError(const std::string &msg, int errorCode)
		: std::runtime_error(
			msg + " (" + std::to_string(errorCode) + ": " + std::string(std::strerror(errno)) + ")"
		), 
		  _errorCode(errorCode)
	{
	}

	int GetErrorCode() const {
		return _errorCode;
	}

private:
	int _errorCode;
};

#endif //__NETWORKERROR_H__