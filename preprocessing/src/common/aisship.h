
#ifndef __AISSHIP_H_INCLUDED__
#define __AISSHIP_H_INCLUDED__



#include <iostream>
#include <vector>
#include <chrono>
#include "coordinate.h"

#include <GeographicLib/Geodesic.hpp>


class AISShip
{
public:
	AISShip();
	~AISShip();
	void addSpeed();
	void calcSpeed();
	void addCoordinateWGS84Timed(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, double lon, double lat);

	void addCompassInfo(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, double ROT, double SOG, double COG, double Heading);
	void addMetadata(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, double lon, double lat, double ROT, double SOG, double COG, double Heading, std::string& type_of_mobile, 
		std::string& Navigational_status, std::string& IMO, std::string& Callsign, std::string& Name,
		std::string& Ship_type, std::string& Cargo_type, std::string& Width, std::string& Length, std::string& Type_of_position_fixing_device,
		std::string& Draught, std::string& Destination, std::string& ETA, std::string& Data_source_type);

	void dumpCSV(const char* basepath);
	std::string MMSI;
	std::vector<Coordinate> positions;

	  std::vector<std::chrono::time_point<std::chrono::high_resolution_clock> > time_points;

	std::vector<double> speeds;
	std::vector<double> deltats;
	std::vector<double> reltime;
	std::vector<double> distances;
	int pixi; // offset variable from skipping elements in the output array, relative to the input array. Sorry for the mess...

};


#endif // __AISSHIP_H_INCLUDED__
