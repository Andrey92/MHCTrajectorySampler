#ifndef BAD_CONF_H
#define BAD_CONF_H

namespace exceptions {

	/*
	 * A BadConf exception is thrown when format of
	 * conf file is not valid.
	 */

	class BadConf: public exception {

		public:
			virtual const char* what() const throw() {
				return "Bad conf file format";
			}

	};

}

#endif
