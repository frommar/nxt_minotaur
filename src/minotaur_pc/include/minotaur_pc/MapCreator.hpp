#ifndef MAPCREATOR_HPP_
#define MAPCREATOR_HPP_

#include "minotaur_pc/Map.hpp"
#include "minotaur_pc/RobotPosition.hpp"

#define RAD_TO_DEGREE 57.295779513082
#define DEGREE_TO_RAD 0.017453292519943

namespace minotaur
{
  class MapCreator
  {
  private:
    Map map;
    int grid[500][500];
  public:
    MapCreator();
    MapCreator(int width, int height);
    MapCreator(Map p_map);
    ~MapCreator();
    void setSensorDistances(int sensor, int x, int y);
    void step(int sensorValue1, int sensorValue2, int sensorValue3);
    void calculateObstaclePosition(int measuredDistance, const int sensor);
    void setPosition(RobotPosition p_position);
    Map getMap();
    void createTextFile();
    void createTextFile2();
  };
}

#endif