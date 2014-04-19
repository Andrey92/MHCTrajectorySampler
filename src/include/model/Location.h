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

	class Location {

		private:
			// Location name
			string name;
			// Position of the top-left corner of the location
			Point position;
			// Width and height
			double width, height;
			// List of cells included in the location.
			// A cell belongs to a location if its center is included in it.
			list<Cell> cells;

		public:
			// Constructors
			Location();
			Location(const string&);
			Location(const string&, double, double, double, double);
			Location(const string&, const Point& p, double, double);

			// Getter methods
			virtual const string& getName(void) const;
			virtual const Point& getPos(void) const;
			virtual double getWidth(void) const;
			virtual double getHeight(void) const;

			// Adds a cell to the cells list
			virtual void add(Cell& c);

			// Number of cells in the location
			virtual unsigned int nCells(void) const;

			// Verifies if p is contained in the location
			virtual bool contains(Point& p) const;

			// Iterators over cells
			virtual list<Cell>::iterator begin(void);
			virtual list<Cell>::iterator end(void);

	};

}

#endif
