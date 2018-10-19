#include "aisreader.h"

#include <iostream>
#include <string>
#include "csv.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

#include "aisship.h"


AISReader::AISReader(char* filename)
{
    std::cout << "Hello world!!!" << std::endl;

// # Timestamp,Type of mobile,MMSI,Latitude,Longitude,Navigational status,ROT,SOG,COG,Heading,IMO,Callsign,Name,Ship type,Cargo type,Width,Length,Type of position fixing device,Draught,Destination,ETA,Data source type
// 01/06/2018 00:00:00,Class A,215810000,55.921028,17.320418,Constrained by her draught,0.0,12.5,255.5,256,Unknown,,,Undefined,,,,Undefined,,,,AIS


    std::map<string,AISShip> ships;


	io::CSVReader<22,  io::trim_chars<' ', '\t'>, io::double_quote_escape<',', '\"'>> in(filename);
	in.read_header(io::ignore_extra_column, "# Timestamp","Type of mobile","MMSI","Latitude","Longitude","Navigational status","ROT","SOG","COG","Heading","IMO","Callsign","Name","Ship type","Cargo type","Width","Length","Type of position fixing device","Draught","Destination","ETA","Data source type");

	std::string sTimestamp, MMSI, type_of_mobile, Navigational_status,IMO,Callsign,Name,Ship_type,Cargo_type,Width,Length,Type_of_position_fixing_device,Draught,Destination,ETA,Data_source_type;
	double Latitude, Longitude, ROT,SOG,COG;
	int Heading;
	std::tm tm = {};


	while(in.read_row(sTimestamp, type_of_mobile, MMSI, Latitude,Longitude,Navigational_status,ROT,SOG,COG,Heading,IMO,Callsign,Name,Ship_type,Cargo_type,Width,Length,Type_of_position_fixing_device,Draught,Destination,ETA,Data_source_type)){
		
		if (ships.count(MMSI))
		{
			cout << "Ship found: "  << ships.at(MMSI).MMSI << '\n';
		} else {

			AISShip ship;
			ship.MMSI = MMSI;
			ships[MMSI] = ship;
		}

		// std::cout << MMSI << " ";// do stuff with the data
		
		// std::stringstream ss(sTimestamp);
		// ss >> std::get_time(&tm, "%d/%m/%Y %H:%M:%S");
		// auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}
	std::cout << 'Done parsing file.\n';


}

AISReader::~AISReader()
{
}

