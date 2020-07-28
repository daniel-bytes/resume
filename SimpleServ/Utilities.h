#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <map>
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

	inline std::string toKeyValuePair(const std::map<std::string, std::string> &headers) {
		return std::accumulate(
			std::next(headers.begin()), 
			headers.end(),
			std::string(),
			[](std::string acc, std::pair<std::string, std::string> h) {
				return std::move(acc) + (acc.size() == 0 ? "" : "&") + h.first + "=" + h.second;
			}
		);
	}
}

#endif //__UTILITIES_H__