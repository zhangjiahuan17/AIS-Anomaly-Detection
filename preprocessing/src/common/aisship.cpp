#include "aisship.h"
#include "gdal/ogrsf_frmts.h"
#include <GeographicLib/LambertConformalConic.hpp> 
#include <ctime>
#include <iostream>
#include <GeographicLib/Geodesic.hpp>
#include <chrono>
#include <fstream>

using namespace std;
using namespace GeographicLib;

void
AISShip::addSpeed() {
  const Geodesic& geod = Geodesic::WGS84();
  if (positions.size() > 1) {
    int pix = positions.size() - 1;

    std::chrono::duration<double> dt = positions.at(pix).datetime - positions.at(pix-1).datetime; // casting to seconds
    double ds;
    geod.Inverse(positions.at(pix-1).lat, positions.at(pix-1).lon, positions.at(pix).lat, positions.at(pix).lon, ds);
    // cout << positions.at(pix-1).lat << " " << positions.at(pix-1).lon << " " << positions.at(pix).lat <<  " " << positions.at(pix).lon << endl;
    double speed = ds/dt.count(); // m/s
  


    // Logical case
    // If I get an update where only time has incremented, but previously a movement was deterimned, I will wait and possibly adjust based on the next movement
    
    if (speeds.at(pix-1-pixi) < 0.01 and deltats.at(pix-1-pixi) > 0.01) {
      double speed_corrected = ds / (dt.count() + deltats.at(pix-1-pixi));
      if (speeds.size() > 2) {
          cout << "corrected speed= " << speed_corrected << " Ratio to previous: " << speed_corrected / speeds.at(pix-2-pixi) <<  endl;
        if (((speed_corrected / speeds.at(pix-2-pixi)) > 0.6) and ((speed_corrected / speeds.at(pix-2-pixi)) < 1.6) ) {

          speeds.at(pix-1-pixi) = speed_corrected;
          distances.at(pix-1-pixi) = ds;
          deltats.at(pix-1-pixi) = deltats.at(pix-1-pixi) + dt.count();
          reltime.at(pix-1-pixi) = reltime.at(pix-1-pixi) + dt.count();
          pixi += 1;
          cout << std::fixed << reltime.back() << ": dt=" << deltats.back() << " ds=" << distances.back() << " m/s=" << speeds.back()  <<  endl;

          return;
        }
      }      
    }


    // Handle cases
    if (dt.count() < 0.01 and ds < 0.01) { // Update without position or time difference, irgnored
    //   speed = 0;
      pixi += 1;
      return;
    }

    if (dt.count() > 0.01 and ds < 0.0001) { // Update not containing position information
      cout << "Save for later correction\n";
      speeds.push_back(speed);
      distances.push_back(ds);
      deltats.push_back(dt.count());
      reltime.push_back(reltime.back() + dt.count());
      return;
    }

    speeds.push_back(speed);
    distances.push_back(ds);
    deltats.push_back(dt.count());
    reltime.push_back(reltime.back() + dt.count());
    cout << std::fixed << reltime.back() << ": dt=" << deltats.back() << " ds=" << distances.back() << " m/s=" << speeds.back()  <<  endl;
  }


}
void
AISShip::calcSpeed(){

  const Geodesic& geod = Geodesic::WGS84();
  if (positions.size() > 1) {
    for (int pix = 1; pix < positions.size(); ++pix)
    {
      std::chrono::duration<double> dt = positions.at(pix).datetime - positions.at(pix-1).datetime; // casting to seconds
      double ds;
      geod.Inverse(positions.at(pix-1).lat, positions.at(pix-1).lon, positions.at(pix).lat, positions.at(pix).lon, ds);
      // cout << positions.at(pix-1).lat << " " << positions.at(pix-1).lon << " " << positions.at(pix).lat <<  " " << positions.at(pix).lon << endl;
      double speed = ds/dt.count(); // m/s
    


      // Logical case
      // If I get an update where only time has incremented, but previously a movement was deterimned, I will wait and possibly adjust based on the next movement
      
      if (speeds.at(pix-1-pixi) < 0.01 and deltats.at(pix-1-pixi) > 0.01) {
        double speed_corrected = ds / (dt.count() + deltats.at(pix-1-pixi));
        if (speeds.size() > 2) {
            // cout << "corrected speed= " << speed_corrected << " Ratio to previous: " << speed_corrected / speeds.at(pix-2-pixi) <<  endl;
          if (((speed_corrected / speeds.at(pix-2-pixi)) > 0.6) and ((speed_corrected / speeds.at(pix-2-pixi)) < 1.6) ) {

            speeds.at(pix-1-pixi) = speed_corrected;
            distances.at(pix-1-pixi) = ds;
            deltats.at(pix-1-pixi) = deltats.at(pix-1-pixi) + dt.count();
            reltime.at(pix-1-pixi) = reltime.at(pix-1-pixi) + dt.count();
            pixi += 1;
            // cout << std::fixed << reltime.back() << ": dt=" << deltats.back() << " ds=" << distances.back() << " m/s=" << speeds.back()  <<  endl;

            continue;
          }
        }      
      }


      // Handle cases
      if (dt.count() < 0.01 and ds < 0.01) { // Update without position or time difference, irgnored
      //   speed = 0;
        pixi += 1;
        continue;
      }
      //dt=0.000000 ds=0.000000 km/h=-nan

      // if (speeds.size() > 1) {
      //   if (speed/speeds.at(pix-1))
      //   cout << "prev speed = " << speeds.at(pix-1) << endl; 
      // }      

      if (dt.count() > 0.01 and ds < 0.0001) { // Update not containing position information
        // cout << "Save for later correction\n";
        speeds.push_back(speed);
        distances.push_back(ds);
        deltats.push_back(dt.count());
        reltime.push_back(reltime.back() + dt.count());
        continue;
      }
      // dt=6.000000 ds=52.493264 km/h=31.495959
      // dt=5.000000 ds=0.000000 km/h=0.000000
      // dt=1.000000 ds=52.355754 km/h=188.480714
      // dt=6.000000 ds=52.421736 km/h=31.453041

      speeds.push_back(speed);
      distances.push_back(ds);
      deltats.push_back(dt.count());
      reltime.push_back(reltime.back() + dt.count());
      // cout << std::fixed << reltime.back() << ": dt=" << deltats.back() << " ds=" << distances.back() << " m/s=" << speeds.back()  <<  endl;

    }

}
  

}
void
AISShip::addCoordinateWGS84Timed(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, double lon, double lat) {
	positions.push_back(Coordinate{.lon=lon, .lat=lat, .datetime=tp});
  // addSpeed();
}

void
AISShip::addCompassInfo(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, 
  double ROT, double SOG, double COG, double Heading) {

}

void
AISShip::addMetadata(std::chrono::time_point<std::chrono::high_resolution_clock>& tp, 
  double lon, double lat, double ROT, double SOG, double COG, double Heading, 
  std::string& type_of_mobile, std::string& Navigational_status, std::string& IMO, 
  std::string& Callsign, std::string& Name, std::string& Ship_type, std::string& Cargo_type, 
  std::string& Width, std::string& Length, std::string& Type_of_position_fixing_device,
  std::string& Draught, std::string& Destination, std::string& ETA, std::string& Data_source_type) {


  time_points.push_back(tp);

  addCoordinateWGS84Timed(tp, lon, lat);
  addCompassInfo(tp, ROT, SOG, COG, Heading);
}

void
AISShip::dumpCSV(const char*  basepath) {

      std::ofstream fo;
      fo.open ("/tmp/fil" +  std::string(MMSI) + ".csv");
      fo << "This is the first cell in the first column.\n";
      for (int i = 0; i < reltime.size(); ++i)
      {

        std::time_t t = std::chrono::system_clock::to_time_t(time_points.at(i));


        fo << std::ctime(&t) << ",";
        fo << reltime.at(i) << ",";
        fo << speeds.at(i) << ",";
        fo << "\n";
      }
      fo.close();


}


AISShip::AISShip() : speeds{0}, deltats{0}, reltime{0}, distances{0}, pixi(0){}
AISShip::~AISShip(){}