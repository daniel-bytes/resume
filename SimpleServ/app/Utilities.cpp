#include "Utilities.h"

using namespace std;

/**
 * Utility functions
 */
namespace Utilities {

	vector<string> Split(const string &line, char delim) {
		string current;
		vector<string> results;

		for (auto c : line) {
			if (c == delim) {
				if (current.size() > 0) {
					results.push_back(current);
					current = "";
				}
			} else {
				current += c;
			}
		}

		if (current.size() > 0) {
			results.push_back(current);
		}

		return results;
	}
}
