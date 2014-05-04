#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "../include/util/Util.h"

using namespace std;

namespace util {

	/*
	 * Implementation of Util class.
	 */

	const char Util::BLANKS[] = " \n\r\t";

	string& Util::trim(string& s) {
		s.erase(s.find_last_not_of(BLANKS) + 1);
		s.erase(0, s.find_first_not_of(BLANKS));
		return s;
	}

	vector<string> Util::split(const string& s, char delim) {
		stringstream ss(s);
		string tmp;
		vector<string> v;
		while (getline(ss, tmp, delim)) {
			if (trim(tmp) != "") v.push_back(tmp);
		}
		return v;
	}

	bool Util::comparePairs(pair<double, double> a, pair<double, double> b) {
		return a.second < b.second;
	}

	template<class T>
	T** Util::allocateMatrix(unsigned int r, unsigned int c) {
		if (r <= 0 || c <= 0) throw invalid_argument("Invalid size");
		// This matrix implementation uses contiguous memory
		T** m = new T*[r];
		m[0] = new T[r * c];
		for (unsigned int i = 1; i < r; i++)
			m[i] = m[0] + i * c;
		return m;
	}

	template<class T>
	void Util::deallocateMatrix(T** m) {
		delete[] m[0];
		delete[] m;
	}

	template<class T>
	void Util::fillMatrix(T** m, unsigned int r, unsigned int c, T v) {
		for (unsigned int i = 0; i < r; i++) {
			for (unsigned int j = 0; j < c; j++) {
				m[i][j] = v;
			}
		}
	}

	// Needed by C++ compiler to generate template functions
	template bool** Util::allocateMatrix<bool>(unsigned int, unsigned int);
	template unsigned int** Util::allocateMatrix<unsigned int>(unsigned int, unsigned int);
	template double** Util::allocateMatrix<double>(unsigned int, unsigned int);
	template void Util::deallocateMatrix<bool>(bool**);
	template void Util::deallocateMatrix<unsigned int>(unsigned int**);
	template void Util::deallocateMatrix<double>(double**);
	template void Util::fillMatrix<bool>(bool**, unsigned int, unsigned int, bool);
	template void Util::fillMatrix<unsigned int>(unsigned int**, unsigned int, unsigned int, unsigned int);
	template void Util::fillMatrix<double>(double**, unsigned int, unsigned int, double);

}
