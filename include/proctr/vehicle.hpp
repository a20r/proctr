#ifndef PROCTR_VEHICLE_HPP
#define PROCTR_VEHICLE_HPP

class Vehicle
{
    public:
        Vehicle();
        ~Vehicle();
        Vehicle(int cap, int n_pass, vector<GeoPoint> route);
}

#endif
