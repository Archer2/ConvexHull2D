/**
* Module containing point read and hull print functions. Very small module, but that is so I can get practice
* writing programs with multiple modules
*
* Author: Allen Briggs
* Date: 08/19/2023
*/

module; // Initiate Global Module Fragment (avoids compiler warning C5201 on the include)

// STL Includes that cannot be imported as header modules
#include <string>
#include <memory>

export module FileIO;

import Types;
import <vector>; // Header import - <vector> can be imported, but not all STL headers

export
{
	/**
	* Reads Points from a given file
	* 
	* a_fileName: file (including extension) to read from
	* 
	* return: vector containing all points read from that file
	* 
	* Assumes text file in the following format:
	*	- Line 1:		Count of points (subsequent lines) in the file
	*	- Lines 2-EOF:	Individual Point on each line, defined as 2 values, an X and Y, separated by a space
	*/
	std::unique_ptr<std::vector<Point>> ReadPointsFromFile(std::string a_fileName);

	/**
	* Print given vector of of Points. If no target file is given for output prints to cout.
	* 
	* a_points: vector of Points to print
	* a_outputFile (optional): name (including extension) of file to write to. All contents will be overwritten
	* 
	* Output format is the same as expected input for ReadPointsFromFile:
	*	- Line 1:		Count of points (subsequent lines) in the file
	*	- Lines 2-EOF:	Individual Point on each line, defined as 2 values, an X and Y, separated by a space
	*/
	void PrintPoints(const std::vector<Point>& a_points, std::string a_outputFile = "");
}