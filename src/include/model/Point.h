#ifndef POINT_H
#define POINT_H

namespace model {

	/*
	 * A simple 2D Point.
	 *
	 * In this project, the origin in a map is located on top-left corner,
	 * with axes directed left-to-right (x) and top-to-bottom (y), as follow:
	 *
	 *         x
	 *    o---->
	 *    |
	 *    |
	 *  y v
	 * 	
	 */

	class Point {

		private:
			// 2D Point coordinates
			double x, y;

		public:
			// Constructors
			Point();
			Point(double, double);
			Point(const Point&);

			// Horizontal distance from origin.
			virtual double getX(void) const;

			// Vertical distance from origin.
			virtual double getY(void) const;

			// Euclidean distance between this point and the argument
			virtual double distance(const Point&) const;

	};

}

#endif
