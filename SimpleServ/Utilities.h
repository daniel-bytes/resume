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

	inline std::string readFileIntoString(const std::string &path) {
		std::ifstream f(path);
		std::string result((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		f.close();

		return result;
	}

}

#endif //__UTILITIES_H__