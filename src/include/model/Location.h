#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <list>
#include <iterator>

#include "Cell.h"
#include "Point.h"

using namespace std;

namespace model {

	/*
	 * A Location is a rectangular area of the map.
	 *
	 * It is represented by a position (a couple of coordinates 'x' and 'y'), a width and a height.
	 * See "include/model/Point.h" file for informations about frame of reference used.
	 *
	 *  (x,y)
	 *    o--------o
	 *    |        |
	 *    |   L    | height
	 *    |        |
	 *    o--------o
	 *      width
	 *
	 */

	class Location : public Identifiable {

		// Rename types (hides container implementation)
		typedef list<Cell> Container;
		typedef Container::const_iterator const_iterator;

		private:
			// Location id
			unsigned int id;
			// Location name
			string name;
			// Position of the top-left corner of the location
			Point position;
			// Width and height
			double width, height;
			// Latency constraint of the location
			unsigned int latency;
			// Maximum Traveling Time Constraints (from and to other locations)
			unsigned int maxTimeFrom, maxTimeTo;
			// Cells included in the location:
			// A cell belongs to a location if its center is included in it.
			Container cells;

		public:
			// Constructors
			Location();
			Location(unsigned int);
			Location(unsigned int, const string&);
			Location(unsigned int, const string&, double, double, double, double);
			Location(unsigned int, const string&, const Point&, double, double);
			Location(unsigned int, const string&, double, double, double, double, unsigned int);
			Location(unsigned int, const string&, const Point&, double, double, unsigned int);

			// Getter methods
			virtual unsigned int getId(void) const;
			virtual const string& getName(void) const;
			virtual const Point& getPos(void) const;
			virtual double getWidth(void) const;
			virtual double getHeight(void) const;
			virtual unsigned int getLatency(void) const;
			virtual unsigned int getMaxTimeFrom(void) const;
			virtual unsigned int getMaxTimeTo(void) const;

			// Set latency constraint
			virtual void setLatency(unsigned int);

			// Set maximum Traveling Time constraints
			virtual void setMaxTimeFrom(unsigned int);
			virtual void setMaxTimeTo(unsigned int);

			// Adds a cell to the cells list
			virtual void add(const Cell&);

			// Number of cells in the location
			virtual unsigned int cellCount(void) const;

			// Verify if a point is contained in the location
			virtual bool contains(const Point&) const;
			virtual bool contains(double, double) const;

			// Iterators over cells
			virtual const_iterator begin(void) const;
			virtual const_iterator end(void) const;

	};

}

#endif
