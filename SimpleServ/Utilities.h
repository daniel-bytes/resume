#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

namespace Utilities {

	inline std::string GetEnvVar(const char *env) {
		auto result = std::getenv(env);
		return result ? std::string(result) : std::string("");
	}

	inline int ParseInt(const char *str, int defaultValue) {
		int result;
		std::istringstream iss(str);
		iss >> result;

		if (iss.fail()) {
			result = defaultValue;
		}

		return result;
	}

	inline std::string ToLowerCase(const std::string &str) {
		std::string output;

		for (char c : str) {
			output += std::tolower(c);
		}

		return output;
	}

	inline std::string ToKeyValuePair(const std::unordered_map<std::string, std::string> &headers) {
		std::string output;

		for (auto header : headers) {
			output += (output.size() ? "&" : "") + header.first + "=" + header.second;
		}

		return output;
	}
}

#endif //__UTILITIES_H__