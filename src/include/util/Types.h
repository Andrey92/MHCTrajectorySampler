#ifndef TYPES_H
#define TYPES_H

#include <list>
#include <unordered_map>

#include "../model/Location.h"
#include "../model/Reader.h"

using namespace std;
using namespace model;

namespace util {

	typedef list<unsigned int> LocationSet;
	typedef vector<unsigned int> LocationRandomAccess;
	typedef list<unsigned int> ReaderSet;
	typedef list<unsigned int> IdSet;
	typedef LocationSet::const_iterator LocationIterator;
	typedef LocationRandomAccess::const_iterator LocationRandomIterator;

	typedef pair<string, unsigned int> NameIdPair;
	typedef unordered_map<string, unsigned int> NameIdMap;

	typedef pair<ReaderSet, unsigned long> Reading;
	typedef vector<Reading> Readings;

}

#endif
