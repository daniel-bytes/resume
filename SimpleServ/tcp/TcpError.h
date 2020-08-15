#ifndef __NETWORKERROR_H__
#define __NETWORKERROR_H__

#include <errno.h>
#include <stdexcept>
#include <sstream>
#include <cstring>

#include "Typedefs.h"

class TcpError
	: public std::runtime_error
{
public:
	TcpError(const std::string &msg)
		: TcpError(msg, errno) {}

	TcpError(error_code_t errorCode)
		: std::runtime_error(
			std::to_string(errorCode) + ": " + std::string(std::strerror(errno)) 
		), 
		  _errorCode(errorCode)
	{
	}
	
	TcpError(const std::string &msg, error_code_t errorCode)
		: std::runtime_error(
			msg + " (" + std::to_string(errorCode) + ": " + std::string(std::strerror(errno)) + ")"
		), 
		  _errorCode(errorCode)
	{
	}

	error_code_t GetErrorCode() const {
		return _errorCode;
	}

private:
	error_code_t _errorCode;
};

#endif //__NETWORKERROR_H__