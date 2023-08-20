/**
* Implementation of simple Point file reading and Hull printing
*
* Author: Allen Briggs
* Date: 08/19/2023
*/

module; // Initiate Global Module Fragment explicitly (avoids compiler warning C5201 on the include)

// Includes should not be exported, since this is not a Module Interface file - TODO: Ensure this is the case
#include <string> // Redeclaring this in the implementation seems like it should be unnecessary, but I can't seem to get around it (other than the C++23 std module)
#include <memory> // Redeclaring this in the implementation seems like it should be unnecessary, but I can't seem to get around it (other than the C++23 std module)
#include <fstream>
#include <iostream>

// Declare Implementation Module Fragment
module FileIO;

std::unique_ptr<std::vector<Point>> ReadPointsFromFile(std::string a_fileName)
{
	std::unique_ptr<std::vector<Point>> points = std::make_unique<std::vector<Point>>();

	// Open file
	std::ifstream input;
	input.open(a_fileName);
	if (!input.is_open()) 
	{
		std::cerr << "File '" << a_fileName << "' could not be opened" << std::endl;
		return points;
	}

	std::string data;
	std::getline(input, data);
	int pointCount = std::stoi(data);

	points->reserve(pointCount);

	// Read in points as defined, expecting a simplistic, restrictive format
	for (int i = 0; i < pointCount; i++)
	{
		Point point = {};

		std::getline(input, data, ' '); // Assume a space between X and Y
		point.X = std::stof(data);
		std::getline(input, data);
		point.Y = std::stof(data);

		points->push_back(point);
	}

	return points;
}

void PrintPoints(const std::vector<Point>& a_points, std::string a_outputFile /* = "" */)
{
	// Print either to cout or file
	if (a_outputFile.empty())
	{
		std::cout << a_points.size() << std::endl;
		for (const Point& point : a_points)
		{
			std::cout << point.X << " " << point.Y << std::endl;
		}
	}
	else
	{
		std::ofstream output(a_outputFile);
		output << a_points.size() << std::endl;
		for (const Point& point : a_points)
		{
			output << point.X << " " << point.Y << std::endl;
		}
	}
}