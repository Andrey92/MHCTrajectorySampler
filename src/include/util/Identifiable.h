#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

namespace util {

	/*
	 * An Identifiable is an object provided with an id which identifies every instance.
	 *
	 * An id is an unsigned integer useful to allow objects search in constant time,
	 * for example using arrays indexed by id.
	 */

	class Identifiable {

		public:

			// Gets the object id.
			virtual unsigned int getId(void) const = 0;

	};

}

#endif
