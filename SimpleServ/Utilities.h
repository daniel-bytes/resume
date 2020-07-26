#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <string>
#include <fstream>
#include <sstream>

namespace Utilities {

	inline int parseInt(const char *str, int defaultValue) {
		int result;
		std::istringstream iss(str);
		iss >> result;

		if (iss.fail()) {
			result = defaultValue;
		}

		return result;
	}
}

#endif //__UTILITIES_H__