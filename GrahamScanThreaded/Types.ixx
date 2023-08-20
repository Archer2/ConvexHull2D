/**
* Module containing types used for the Graham's Scan algorithm.
* Very small for a module, but I wanted to experiment with multiple modules, and C++20 recommends using ONLY modules,
* which includes basic files like this
* 
* Author: Allen Briggs
* Date: 08/19/2023
*/

export module Types;

import <stack>; // Header import - <stack> can be imported, but not all STL headers. <stack> gets std::pair defined

export 
{
	/**
	* Represents a 2D Point in the XY plane. Contains only floating point member representation and basic functionality
	*/
	struct Point
	{
		float X;
		float Y;

		bool operator==(const Point& other) { return this->X == other.X && this->Y == other.Y; }
		bool operator!=(const Point& other) { return !operator==(other); }

		static float DistanceSquared(const Point& p1, const Point& p2)
		{
			return ((p2.X - p1.X) * (p2.X - p1.X)) + ((p2.Y - p1.Y) * (p2.Y - p1.Y));
		}

		float DistanceSquared(const Point& other)
		{
			return DistanceSquared(*this, other);
		}
	};

	// Using Edges makes the Graham's Scan easier to read, but it is not necessary to be its own complex type
	using Edge = std::pair<Point, Point>;

	/**
	* Takes a directed Edge and a Point and tests if tthe cross product of that Edge TO that Point is positive. Points
	* are assumed to be in the XY plane.
	*
	* a_edge: directed Edge, of .first -> .second
	* a_testPoint: third Point of the test, of a_edge.second -> a_testPoint
	*
	* return: true if Point is to the positive side of the Edge, false if not
	*/
	bool IsSidePositive(Edge a_edge, Point a_testPoint);

	/**
	* Takes a stack of Points to perform Side test on working set of the Graham's Scan. All points are assumed to be in
	* the XY plane.
	*
	* a_stack: stack of Points, only the top two values of which are accessed, so must have at least two values
	* a_testPoint: third Point of the test, targeted to discover which side of the line formed by other Points it is on
	*
	* return: true if Point is to the positive side of the Edge formed by the top two Points on the stack, false if not
	*/
	bool IsSidePositive(std::stack<Point> a_stack, Point a_testPoint);
}