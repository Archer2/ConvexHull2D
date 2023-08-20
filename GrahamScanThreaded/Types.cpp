/**
* Implementation of Type module functions
*
* Author: Allen Briggs
* Date: 08/19/2023
*/

// Declare Implementation Module Fragment
module Types;

bool IsSidePositive(Edge a_edge, Point a_testPoint)
{
	float vecEdge[2] = {
		a_edge.second.X - a_edge.first.X,
		a_edge.second.Y - a_edge.first.Y
	};
	float vecToPoint[2] = {
		a_testPoint.X - a_edge.second.X,
		a_testPoint.Y - a_edge.second.Y
	};

	// Check Z component of cross product. If it equals 0, then Z axis was an input vector (error input - unchecked)
	return (vecEdge[0]*vecToPoint[1] - vecEdge[1]*vecToPoint[0]) > 0;
}

bool IsSidePositive(std::stack<Point> a_stack, Point a_testPoint)
{
	if (a_stack.size() < 2)
		return false;

	Point top = a_stack.top();
	a_stack.pop();
	Point prevTop = a_stack.top();
	return IsSidePositive(Edge(prevTop, top), a_testPoint);
}