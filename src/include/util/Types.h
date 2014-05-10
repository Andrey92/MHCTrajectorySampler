#include <list>
#include <unordered_map>

#include "../model/Location.h"
#include "../model/Reader.h"

using namespace std;
using namespace model;

namespace util {

	typedef list<Location> LocationSet;
	typedef list<Reader> ReaderSet;
	typedef list<unsigned int> IdSet;

	typedef pair<string, unsigned int> NameIdPair;
	typedef unordered_map<string, unsigned int> NameIdMap;

	typedef pair<IdSet, unsigned long> Reading;
	typedef list<Reading> Readings;

}
