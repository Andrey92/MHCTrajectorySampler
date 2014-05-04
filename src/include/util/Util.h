#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace util {

	/*
	 * Simple utility class.
	 */
	class Util {

		private:
			// Blank characters to trim
			static const char BLANKS[];

			Util() { }

		public:
			// Removes leading and trailing blank characters from s.
			// string s is modified and returned. 
			static string& trim(string& s);

			// Split s into substrings using delim.
			// Each substring is a part of s delimited by two delim characters (or by start/end of s).
			// Empty substrings are deleted.
			static vector<string> split(const string& s, char delim);

			// Compares rate-radius pairs comparing radius values.
			static bool comparePairs(pair<double, double>, pair<double, double>);

			// Allocates and returns a matrix of parametric type T,
			// with specified number of rows and columns.
			template<class T>
			static T** allocateMatrix(unsigned int, unsigned int);

			// Deallocates a matrix of parametric type T created with allocateMatrix method.
			template<class T>
			static void deallocateMatrix(T**);

			// Fills a matrix of parametric type T with a specified value.
			template<class T>
			static void fillMatrix(T**, unsigned int, unsigned int, T);

	};

}

#endif
