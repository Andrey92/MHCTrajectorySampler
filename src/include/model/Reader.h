#ifndef READER_H
#define READER_H

#include <string>
#include <vector>
#include <utility>

#include "Point.h"
#include "../util/ConfLoader.h"
#include "../util/Identifiable.h"

using namespace std;
using namespace util;

namespace model {

	/*
	 * An RFID reader object.
	 *
	 * This model uses reader of the same type (same ReaderModel for each reader).
	 * 
	 * Each reader has a specified position in the map, and its detection rate is
	 * estimated according to the model used.
	 * 
	 * Currently only 'n-state' model is supported.
	 */

	enum class ReaderModel : char { nstate };

	class Reader : public Identifiable {
		
		private:
			// Static variables (shared by all readers)
			// whose values are loaded from conf file
			// Detection model of the readers
			static ReaderModel rModel;
			// Parameters of n-state model: rate-radius value pairs
			static vector<pair<double, double>> states;

			// Reader id
			unsigned int id;
			// Reader name
			string name;
			// Position of the reader
			Point position;

		public:
			// Constructors
			Reader();
			Reader(unsigned int);
			Reader(unsigned int, const string&);
			Reader(unsigned int, const string&, double, double);
			Reader(unsigned int, const string&, const Point&);

			// Friend method of class ConfLoader, can access private static members
			// to set them while loading configuration
			friend void ConfLoader::loadConf(const string& file);

			// Static getter methods
			static ReaderModel getModel(void);
			static unsigned int nStates(void);

			// Getter methods
			virtual unsigned int getId(void) const;
			virtual const string& getName(void) const;
			virtual const Point& getPos(void) const;
			virtual double getMaxRadius(void) const;

			// Returns the detection rate for a given position
			virtual double getRate(const Point& p) const;

			// Iterators over states of the n-state model
			static vector<pair<double, double>>::iterator begin(void);
			static vector<pair<double, double>>::iterator end(void);

	};

}

#endif
