#include "road.h"

float getLaneDistance(json sensor_fusion, double car_s, int lane, int prev_size)
{
    float distance = -1;
    for (int i = 0; i < sensor_fusion.size(); ++i)
    {
        float d = sensor_fusion[i][6];
        if ((d < (4 * (lane + 1))) && (d > (4 * lane)))
        {
            double vx = sensor_fusion[i][3];
            double vy = sensor_fusion[i][4];
            double check_speed = sqrt(vx * vx + vy * vy);
            double check_car_s = sensor_fusion[i][5];
            check_car_s += ((double)prev_size) * .02 * check_speed;
            if (check_car_s > car_s && (check_car_s - car_s) < 30)
            {
                distance = check_car_s - car_s;
            }
        }
    }
    return distance;
}

bool isLaneFrontBackValid(json sensor_fusion, double car_s, int lane, int prev_size)
{
    float frontLantDistance = getLaneDistance(sensor_fusion, car_s, lane, prev_size);
    if (frontLantDistance < 0 || frontLantDistance >= 30)
    {
        // right back no car
        float backLaneDistance = getLaneDistance(sensor_fusion, car_s - 20, lane, prev_size);
        if (backLaneDistance < 0 || backLaneDistance >= 30)
        {
            return true;
        }
    }
    return false;
}

int getBestLane(json sensor_fusion, double car_s, int lane, int prev_size)
{
    float currentLaneDistance = getLaneDistance(sensor_fusion, car_s, lane, prev_size);
    // no car in front, or car is far away
    if (currentLaneDistance < 0 || currentLaneDistance >= 30)
    {
        return lane;
    }
    // too close, check left or right currentLaneDistance < 30
    if (lane == 0)
    {
        if (isLaneFrontBackValid(sensor_fusion, car_s, lane + 1, prev_size))
        {
            return lane + 1;
        }
    }
    else if (lane == 2)
    {
        if (isLaneFrontBackValid(sensor_fusion, car_s, lane - 1, prev_size))
        {
            return lane - 1;
        }
    }
    else
    {
        if (isLaneFrontBackValid(sensor_fusion, car_s, lane - 1, prev_size))
        {
            return lane - 1;
        }
        if (isLaneFrontBackValid(sensor_fusion, car_s, lane + 1, prev_size))
        {
            return lane + 1;
        }
    }
    return lane;
}