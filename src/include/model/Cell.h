#ifndef CELL_H
#define CELL_H

#include <string>

#include "Point.h"
#include "../util/ConfLoader.h"
#include "../util/Identifiable.h"

using namespace util;

namespace model {

	/*
	 * A cell is the elementar square unit of a map.
	 *
	 * Cell size determines model accuracy and efficiency
	 * (smaller cells lead to more accurate but expensive algorithm).
	 */

	class Cell : public Identifiable {

		private:
			// The size of the cells (in centimeters),
			// initialized later from conf file
			static double size;

			// Cell id
			unsigned int id;

			// The center of the cell
			Point center;

		public:
			// Constructors
			Cell(unsigned int);
			Cell(unsigned int, double, double);
			Cell(unsigned int, const Point&);
			Cell(const Cell&);

			// Friend method of class ConfLoader, can access private static members
			// to set them while loading configuration
			friend void ConfLoader::loadConf(const string& file);

			// Returns cells size
			static double getSize(void);

			// Returns the object id.
			virtual unsigned int getId(void) const;

			// The center of the cell (a cell will be treated as a point)
			virtual const Point& getCenter(void) const;

	};

}

#endif
