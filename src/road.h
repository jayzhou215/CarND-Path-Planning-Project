#ifndef ROAD_H
#define ROAD_H

#include "json.hpp"
#include <vector>

using nlohmann::json;

float getLaneDistance(json sensor_fusion, double car_s, int lane, int prev_size);
int getBestLane(json sensor_fusion, double car_s, int lane, int prev_size);
bool isLaneFrontBackValid(json sensor_fusion, double car_s, int lane, int prev_size);

#endif // ROAD_H