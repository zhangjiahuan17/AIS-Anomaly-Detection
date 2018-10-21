#include "aisreader.h"

#include <iostream>
#include <string>
#include "csv.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>

#include <GeographicLib/Geodesic.hpp>
#include <chrono>
using namespace std;
using namespace GeographicLib;


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


	int i = 0;
	while(in.read_row(sTimestamp, type_of_mobile, MMSI, Latitude,Longitude,Navigational_status,ROT,SOG,COG,Heading,IMO,Callsign,Name,Ship_type,Cargo_type,Width,Length,Type_of_position_fixing_device,Draught,Destination,ETA,Data_source_type)){
		if (i > 100000) break;
		i++;

		std::stringstream ss(sTimestamp);
		ss >> std::get_time(&tm, "%d/%m/%Y %H:%M:%S");
		auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		
		if (ships.count(MMSI))
		{
			// cout << "Ship found: "  << ships.at(MMSI).MMSI << '\n';
			AISShip &ship = ships.at(MMSI);
			ship.addMetadata(tp, Longitude, Latitude, ROT, SOG, COG, Heading, type_of_mobile, Navigational_status,IMO,Callsign,Name,Ship_type,Cargo_type,Width,Length,Type_of_position_fixing_device,Draught,Destination,ETA,Data_source_type);

			// ship.addCoordinateWGS84Timed(Longitude, Latitude, tp);
			// std::cout << "Positions: " << ship.positions.size() << "\n";

		} else {

			AISShip ship;
			// ship.addCoordinateWGS84Timed(Longitude, Latitude, tp);
			// ship.addCompassInfo(tp, ROT,SOG,COG,Heading);
			ship.addMetadata(tp, Longitude, Latitude, ROT, SOG, COG, Heading, type_of_mobile, Navigational_status,IMO,Callsign,Name,Ship_type,Cargo_type,Width,Length,Type_of_position_fixing_device,Draught,Destination,ETA,Data_source_type);
			ship.MMSI = MMSI;

			// Save to map
			ships[MMSI] = ship;

		}

		// std::cout << MMSI << " ";// do stuff with the data
		
	}
	std::cout << 'Done parsing file.\n';

	// const Geodesic& geod = Geodesic::WGS84();

	for (auto& kv : ships) {

		kv.second.calcSpeed();
		kv.second.dumpCSV("/tmp");
	   //  if (kv.second.positions.size() > 100) {
	   //  	for (int pix = 1; pix < kv.second.positions.size(); ++pix)
	   //  	{
	   //  		std::chrono::duration<double> dt = kv.second.positions.at(pix).datetime - kv.second.positions.at(pix-1).datetime;
				// double ds;
				// geod.Inverse(kv.second.positions.at(pix-1).lat, kv.second.positions.at(pix-1).lon, kv.second.positions.at(pix).lat, kv.second.positions.at(pix).lon, ds);
	   //  		cout << "dt=" << dt.count() << " ds=" << ds << " km/h=" << ds/dt.count() * 3.6 <<  endl;
	   //  	}

	   //  cout << endl;
	   //  }
	}
}

AISReader::~AISReader()
{
}

