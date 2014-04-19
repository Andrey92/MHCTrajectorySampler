#include <string>
#include <list>
#include <iterator>

#include "../include/model/Location.h"

using namespace std;

namespace model {

	/*
	 * Implementation of Location class.
	 */

	Location::Location() : name(), position(), width(0), height(0), cells() { }

	Location::Location(const string& name) : name(name), position(), width(0), height(0), cells() { }

	Location::Location(const string& name, double x, double y, double w, double h) :
		name(name), position(x, y), width(w), height(h), cells() { }

	Location::Location(const string& s, const Point& p, double w, double h) :
		name(name), position(p), width(w), height(h), cells() { }

	const string& Location::getName(void) const {
		return name;
	}

	const Point& Location::getPos(void) const {
		return position;
	}

	double Location::getWidth(void) const {
		return width;
	}

	double Location::getHeight(void) const {
		return height;
	}

	void Location::add(Cell& c) {
		cells.push_back(c);
	}

	unsigned int Location::nCells(void) const {
		return cells.size();
	}

	bool Location::contains(Point& p) const {
		return p.getX() >= position.getX() &&
			p.getX() <= (position.getX() + width) &&
			p.getY() >= position.getY() &&
			p.getY() <= (position.getY() + height);
	}

	list<Cell>::iterator Location::begin(void) {
		return cells.begin();
	}
	
	list<Cell>::iterator Location::end(void) {
		return cells.end();
	}

}
