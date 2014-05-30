#ifndef HPFLOAT_H
#define HPFLOAT_H

#include <string>

using namespace std;

namespace util {

	/*
	 * HPDouble class represent an high precision decimal number.
	 *
	 * It uses scientific notation 'm * 10^(e)', with a long double mantissa 'm'
	 * and a long int exponent 'e'.
	 */

	class HPFloat {
		
		private:
			// Mantissa
			long double m;
			// Exponent
			long int e;

			// Private method:
			// Adjust the exponent so that 1 <= abs(m) < 10
			// If m == 0 it does nothing.
			virtual void normalize(void);


		public:
			// Constructors
			HPFloat();
			HPFloat(long double);
			HPFloat(const HPFloat&);

			// Returns a value representation of the number as a long double.
			// Loss of significant digits may occur.
			virtual long double getValue(void) const;

			// Returns a string representation of the number
			virtual const string toString(void) const;

			// Multiply number for a given long double
			virtual void mul(long double);

			// Divide number for a given long double
			virtual void div(long double);

			// Operators * and \ for HPFloat objects
			virtual const HPFloat operator* (const HPFloat&) const;
			virtual const HPFloat operator/ (const HPFloat&) const;
			virtual HPFloat& operator*= (const HPFloat&);
			virtual HPFloat& operator/= (const HPFloat&);

	};

}

#endif
