#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <optional>

/**
 * Utility functions
 */
namespace Utilities {

	/**
	 * Gets and environment variable.
	 */
	inline std::optional<std::string> GetEnvVar(const char *env) {
		auto result = std::getenv(env);
		return result ? std::string(result) : std::optional<std::string>{};
	}

	/**
	 * Parses an integer from a string,
	 * returns a default value if parsing fails.
	 */
	inline std::optional<int> ParseInt(const char *str) {
		int result;
		std::istringstream iss(str);
		iss >> result;

		if (iss.fail()) {
			return std::optional<int> { };
		} else {
			return result;
		}
	}

  /**
	 * Convers a string to lower case
	 */
	inline std::string ToLowerCase(const std::string &str) {
		std::string output;

		for (char c : str) {
			output += std::tolower(c);
		}

		return output;
	}

  /**
	 * Converts a map to a key=value& string
	 */
	inline std::string ToKeyValuePair(const std::unordered_map<std::string, std::string> &kvps) {
		std::string output;

		for (auto kvp : kvps) {
			output += (output.size() ? "&" : "") + kvp.first + "=" + kvp.second;
		}

		return output;
	}

	/**
	 * Parses a map from a key=value& string
	 */
	inline std::unordered_map<std::string, std::string> FromKeyValuePair(const std::string & kvps) {
		std::unordered_map<std::string, std::string> output;

		// TODO

		return output;
	}
}

#endif //__UTILITIES_H__