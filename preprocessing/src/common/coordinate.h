
#ifndef __COORDINATE_H_INCLUDED__
#define __COORDINATE_H_INCLUDED__

#include <chrono>

struct Coordinate
{
	double lon, lat;
	std::chrono::time_point<std::chrono::high_resolution_clock> datetime;

};

#endif