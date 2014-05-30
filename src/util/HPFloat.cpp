#include <sstream>
#include <iomanip>

#include "../include/util/HPFloat.h"

namespace util {

	/*
	 * Implementation of HPFloat class.
	 */

	void HPFloat::normalize(void) {
		if (m == 0.0) return;
		while (m < 1.0) {
			m *= 10;
			e -= 1;
		}
		while (m >= 10) {
			m /= 10;
			e += 1;
		}
	}

	HPFloat::HPFloat() : m(1.0), e(0) { }

	HPFloat::HPFloat(long double value) : m(value), e(0) {
		normalize();
	}

	HPFloat::HPFloat(const HPFloat& h) : m(h.m), e(h.e) { }

	long double HPFloat::getValue(void) const {
		long double value = m;
		long int exp = e;
		while (exp > 0) {
			value *= 10;
			exp--;
		}
		while (exp < 0) {
			value /= 10;
			exp++;
		}
		return value;
	}

	const string HPFloat::toString(void) const {
		ostringstream ss;
		ss << fixed << setprecision(5) << m;
		if (e != 0) ss << "e" << (e > 0 ? "+" : "") << e;
		return ss.str();
	}

	void HPFloat::mul(long double n) {
		m *= n;
		normalize();
	}

	void HPFloat::div(long double n) {
		m /= n;
		normalize();
	}

	const HPFloat HPFloat::operator* (const HPFloat& f) const {
		return HPFloat(*this) *= f;
	}

	const HPFloat HPFloat::operator/ (const HPFloat& f) const {
		return HPFloat(*this) /= f;
	}

	HPFloat& HPFloat::operator*= (const HPFloat& f) {
		m *= f.m;
		e += f.e;
		normalize();
		return *this;
	}

	HPFloat& HPFloat::operator/= (const HPFloat& f) {
		m /= f.m;
		e -= f.e;
		normalize();
		return *this;
	}

}
