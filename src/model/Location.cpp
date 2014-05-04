#include <string>

#include "../include/model/Location.h"

using namespace std;

namespace model {

	/*
	 * Implementation of Location class.
	 */

	Location::Location() : id(0), name(), position(), width(0), height(0), latency(0), cells() { }

	Location::Location(unsigned int id) : id(id), name(), position(), width(0), height(0), latency(0), cells() { }

	Location::Location(unsigned int id, const string& name) :
		id(id), name(name), position(), width(0), height(0), latency(0), cells() { }

	Location::Location(unsigned int id, const string& name, double x, double y, double w, double h) :
		id(id), name(name), position(x, y), width(w), height(h), latency(0), cells() { }

	Location::Location(unsigned int id, const string& s, const Point& p, double w, double h) :
		id(id), name(name), position(p), width(w), height(h), latency(0), cells() { }

	Location::Location(unsigned int id, const string& name, double x, double y, double w, double h, unsigned int l) :
		id(id), name(name), position(x, y), width(w), height(h), latency(l), cells() { }

	Location::Location(unsigned int id, const string& s, const Point& p, double w, double h, unsigned int l) :
		id(id), name(name), position(p), width(w), height(h), latency(l), cells() { }

	unsigned int Location::getId(void) const {
		return id;
	}

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

	unsigned int Location::getLatency(void) const {
		return latency;
	}

	void Location::setLatency(unsigned int l) {
		latency = l;
	}

	void Location::add(const Cell& c) {
		cells.push_back(c);
	}

	unsigned int Location::cellCount(void) const {
		return cells.size();
	}

	bool Location::contains(const Point& p) const {
		return contains(p.getX(), p.getY());
	}

	bool Location::contains(double x, double y) const {
		return x >= position.getX() &&
			x <= (position.getX() + width) &&
			y >= position.getY() &&
			y <= (position.getY() + height);
	}

	Location::const_iterator Location::begin(void) const {
		return cells.cbegin();
	}
	
	Location::const_iterator Location::end(void) const {
		return cells.cend();
	}

}
