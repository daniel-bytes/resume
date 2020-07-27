#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <numeric>
#include <algorithm>

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

	inline std::string toLower(const std::string &str) {
		return std::accumulate(
			std::next(str.begin()), 
			str.end(),
			std::string(),
			[](std::string acc, char c) {
				auto result = std::move(acc);
				result.push_back(c);
				return result;
			}
		);
	}
}

#endif //__UTILITIES_H__