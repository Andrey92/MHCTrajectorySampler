#ifndef MAP_H
#define MAP_H

#include "Location.h"
#include "Reader.h"

using namespace util;

namespace model {
	
	/*
	 * A map is a set of locations, readers and constraints between locations.
	 */

	class Map {

		private:
			// Set of Locations
			Location* locations;
			// Set of Readers
			Reader* readers;
			// Number of locations, readers and cells
			unsigned int lCount, rCount, cCount;
			// Map size (in centimeters)
			double width, height;
			// Direct (Un)Reachability Constraints matrix
			bool** dr;
			// Traveling Time Constraints matrix
			unsigned int** tt;
			// Detection rates matrix (readers * cells).
			// Each element (r,c) represents the probability P(r|c) that
			// an object is detected by reader 'r' given that it is into cell 'c'.
			double** p;

			// Private initialization methods
			template<class LSet> void initLocations(const LSet&);
			template<class RSet> void initReaders(const RSet&);
			virtual void initConstraints(void);
			virtual void initCells(void);
			virtual void initDetectionRates(void);

			// Private helper methods
			// Returns a pointer to the location containing the specified coordinates,
			// or nullptr if no such location exists.
			virtual Location* getLocation(double, double) const;
			// Checks if a location id is valid
			virtual bool isValid(unsigned int) const;
			// Returns the minimum distance between a location and a reader,
			// or 0 if reader is within the location.
			virtual double minDistance(const Location&, const Reader&) const;

			// Private constructor
			Map() : width(0), height(0) { };

		public:

			typedef const Location* location_iterator;
			typedef const Reader* reader_iterator;
			typedef list<Location> LocationSet;

			// Factory method to create a map object
			// Receives a set of Locations and a set of Readers
			template<class LSet, class RSet>
			static Map* createMap(const LSet&, const RSet&);
			// Destructor
			~Map();

			// Getter methods (constraints)
			virtual unsigned int getLocationCount(void) const;
			virtual unsigned int getReaderCount(void) const;
			virtual unsigned int getCellCount(void) const;
			virtual double getWidth(void) const;
			virtual double getHeight(void) const;
			virtual bool getDR(unsigned int, unsigned int) const;
			virtual bool getDU(unsigned int, unsigned int) const;
			virtual unsigned int getLT(unsigned int) const;
			virtual unsigned int getTT(unsigned int, unsigned int) const;

			// Setter methods (constraints)
			virtual void setDR(unsigned int, unsigned int);
			virtual void setDU(unsigned int, unsigned int);
			virtual void setLT(unsigned int, unsigned int);
			virtual void setTT(unsigned int, unsigned int, unsigned int);

			// Returns the set of likely locations related to a set of reader
			template<class RSet>
			LocationSet* likelyLocations(const RSet&) const;

			// Same as above, for a single reader
			virtual LocationSet* likelyLocations(const Reader&) const;

			// Get the probability P(R|L) that an object is detected by a set 'R' of readers
			// given that it is into location 'L' (likelihood function).
			template<class RSet>
			double prob(const RSet&, const Location&) const;

			// Iterator over locations
			virtual location_iterator lbegin(void) const;
			virtual location_iterator lend(void) const;

			// Iterator over readers
			virtual reader_iterator rbegin(void) const;
			virtual reader_iterator rend(void) const;
	};

}

// Required for template methods.
#include "Map_impl.h"

#endif
