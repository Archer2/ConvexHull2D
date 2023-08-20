/**
* Replication of Graham's Scan Convex Hull algorithm in 2d, using a thread each for the top and bottom to speed calculation.
* 
* Primary goal is to gain proficiency with threading (long past due) and learn C++20 modules (in a basic way, since this program is small)
* 
* Author: Allen Briggs
* Date: 08/19/2023
*/

// Includes - all STL files must be included until full release and support for C++23, when std module is implemented in VS (without c++:latest)
#include <iostream> // import <iostream> // import std // .iostream??
#include <string> // import <string> // import std // .core?
#include <memory> // import <memory> // import std // .core?
#include <algorithm>
#include <thread>
#include <functional>

// STL Header-Imports where possible
import <vector>;
import <stack>;

// Module Imports
import Types;
import FileIO;

/**
* Accepts a set of 2D points on the XY plane and performs the Craham's Scan algorithm to compute the Convex Hull. Outputs Convex Hull
* of a_inputPoints in  aCounter-Clockwise order. Hull is Closed and includes collinear points
* 
* a_inputPoints: unsorted point set P, of which to compute the convex hull
* 
* return: pointer to convex hull of P
* 
* Note: Multithreading this seems to have not improved it at all. For 100,000 inputs it shows only an ~5 second improvement, and for
*		250,000 it shows an ~10 second performance loss. This could be due to naive implementation, or something under the hood (slow
*		context switching?). This is not related to file writing, as cutting that shows only ~4sec gain (and the Hull is rarely more
*		than 50 lines long, if that, even for 250,000 input points)
*/
std::unique_ptr<std::vector<Point>> GrahamScan(const std::vector<Point>& a_inputPoints)
{
	std::vector<Point> pointSet;
	pointSet.reserve(a_inputPoints.size()); // Not sure if reserving is great here, since it isn't guaranteed all points are copied over

	// Remove duplicates (can cause result to have invalid points)
	for (const Point& point : a_inputPoints)
	{
		if (std::find(pointSet.begin(), pointSet.end(), point) == pointSet.end())
			pointSet.push_back(point);
	}

	// Sort on increasing X, and if X is equal use increasing Y
	std::sort(pointSet.begin(), pointSet.end(), [](const Point& a, const Point& b) {
		return a.X == b.X ? a.Y < b.Y : a.X < b.X;
	});

	// Begin algorithm
	Point left = pointSet[0];
	Point right = pointSet[pointSet.size() - 1];;
	std::stack<Point> upperHull, lowerHull;

	// Initial pushes
	upperHull.push(left);
	upperHull.push(pointSet[1]);

	lowerHull.push(pointSet[pointSet.size() - 1]);
	lowerHull.push(pointSet[pointSet.size() - 2]);

	// Shorthand lambda for removing points, since that is same between threads. All that changes is the for loop through points
	auto RemoveInnerPoints = [](std::stack<Point>& workingHull, Point target) {
		while (workingHull.size() >= 2 && IsSidePositive(workingHull, target))
		{
			workingHull.pop();
		}
		workingHull.push(target);
	};
	
	// Launch threads for upper and lower hull loops
#define FRACTIONAL_HULL_LAMBDA_DEC [RemoveInnerPoints](const std::vector<Point>& pointSet, std::stack<Point>& fractionHull)

	std::thread upperThread(FRACTIONAL_HULL_LAMBDA_DEC {
		for (int i = 2; i < pointSet.size(); i++)
			RemoveInnerPoints(std::ref(fractionHull), pointSet[i]);
	}, std::ref(pointSet), std::ref(upperHull));
	
	std::thread lowerThread(FRACTIONAL_HULL_LAMBDA_DEC {
		for (int i = (int)pointSet.size() - 3; i >= 0; i--)
			RemoveInnerPoints(std::ref(fractionHull), pointSet[i]);
	}, std::ref(pointSet), std::ref(lowerHull));

#undef FRACTIONAL_HULL_LAMBDA_DEC

	// Wait for threads to both join (should take roughly the same amount of async time for each)
	// Could do one loop here on the main thread as it is just waiting for both workers
	upperThread.join();
	lowerThread.join();

	// Convert stacks to singular Hull and return
	std::unique_ptr<std::vector<Point>> hullPoints = std::make_unique<std::vector<Point>>();
	hullPoints->reserve(upperHull.size() + lowerHull.size() - 1); // -1 since final point of upperHull is first point of lowerHull
	while (upperHull.size() > 1)
	{
		hullPoints->push_back(upperHull.top());
		upperHull.pop();
	}
	while (lowerHull.size() > 0)
	{
		hullPoints->push_back(lowerHull.top());
		lowerHull.pop();
	}
	return hullPoints;
}

/**
* Accepts filenames to read Points from and write Points to as optional command line elements.
* 
* Input must be in the following format:
*	- Line 1:		Count of points (subsequent lines) in the file
*	- Lines 2-EOF:	Individual Point on each line, defined as 2 values, an X and Y, separated by a space
* 
* argc: count of command line elements in argv
* argv: array of command line arguments. [0] is always the exe run command itself.
*	- argv[1] is treated as a filename (with extension) to read input from
*	- argv[2] is treated as a filename (with extension) to write output to
*/
int main(int argc, char** argv)
{
	// Read input - argv[0] is run command itself
	std::string inputFile = argc >= 2 ? argv[1] : "InputPoints.txt"; // Default filename in case nothing is passed to program
	std::unique_ptr<std::vector<Point>> points = ReadPointsFromFile(inputFile);

	if (points->size() < 3)
	{
		std::cerr << "Error: Input point set must contain at least 3 points" << std::endl;
		return 0;
	}
	
	// Perform algorithm
	std::unique_ptr<std::vector<Point>> hull = GrahamScan(*points);

	// Print results
	std::string outputFile = argc >= 3 ? argv[2] : ""; // Default print to console
	PrintPoints(*hull, outputFile);

	return 0;
}