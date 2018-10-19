
#ifndef __AISSHIP_H_INCLUDED__
#define __AISSHIP_H_INCLUDED__



#include <iostream>
#include <vector>

class AISShip
{
public:
	AISShip();
	~AISShip();

	std::string MMSI;
	std::vector<Coordinate> positions;

};


#endif // __AISSHIP_H_INCLUDED__
