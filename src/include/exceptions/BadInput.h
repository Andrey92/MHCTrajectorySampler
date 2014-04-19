#ifndef BAD_INPUT_H
#define BAD_INPUT_H

namespace exceptions {

	/*
	 * A BadInput exception is thrown when format of
	 * some input (map or constraints) file is not valid.
	 */

	class BadInput: public exception {

		public:
			virtual const char* what() const throw() {
				return "Bad input file format";
			}

	};

}

#endif
