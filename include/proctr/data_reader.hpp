#ifndef PROCTR_DATA_READER_HPP
#define PROCTR_DATA_READER_HPP

#include <nanoflann.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "proctr/prior.hpp"

using namespace nanoflann;

const double earth_radius = 6371;

struct GeoPoint
{
    double  lng, lat;

    GeoPoint(double lng, double lat) : lng(lng), lat(lat)
    {
    }

    GeoPoint()
    {
    }

    GeoPoint(string csv_line)
    {
        stringstream line_stream(csv_line);
        string cell;

        int counter = 0;
        while (getline(line_stream, cell, ','))
        {
            switch (counter++)
            {
                case 1: this->lat = stod(cell);
                case 2: this->lng = stod(cell);
            }
        }
    }

};

class GeoPoints
{
    public:
        std::vector<GeoPoint> pts;

        inline size_t kdtree_get_point_count() const
        {
            return pts.size();
        }

        inline double kdtree_distance(const double *p1, const size_t idx_p2,
                size_t) const
        {
            double lat1 = p1[1] * (M_PI / 180.0);
            double lng1 = p1[0] * (M_PI / 180.0);
            double lat2 = pts[idx_p2].lat * (M_PI / 180.0);
            double lng2 = pts[idx_p2].lng * (M_PI / 180.0);
            double dlng = lng2 - lng1;
            double dlat = lat2 - lat1;
            double a = pow(sin(dlat / 2.0), 2) + cos(lat1) * cos(lat2)
                * pow(sin(dlng / 2.0), 2);
            double c = 2 * asin(sqrt(a));
            double dist_km = earth_radius * c;
            return dist_km;
        }

        inline double kdtree_get_pt(const size_t idx, int dim) const
        {
            if (dim <= 0)
            {
                return pts[idx].lng;
            } else {
                return pts[idx].lat;
            }
        }

        template <class BBOX>
        bool kdtree_get_bbox(BBOX&) const
        {
            return false;
        }
};

struct PickupEvent
{
    GeoPoint pickup, dropoff;
    int pickup_st, dropoff_st;
    ptime pickup_time, dropoff_time;

    PickupEvent()
    {
    }

    PickupEvent(GeoPoint pickup, GeoPoint dropoff,
            int pickup_st, int dropoff_st,
            ptime pickup_time, ptime dropoff_time) :
        pickup(pickup), dropoff(dropoff),
        pickup_st(pickup_st), dropoff_st(dropoff_st),
        pickup_time(pickup_time), dropoff_time(dropoff_time)
    {
    }

    bool operator<(const PickupEvent& other) const
    {
        return pickup_time < other.pickup_time;
    }
};

inline ostream &operator<<(ostream &os, GeoPoint const &geo)
{
    os << "[lng: " << geo.lng << ", lat: " << geo.lat << "]";
    return os;
}

inline ostream &operator<<(ostream &os, PickupEvent const &event)
{
    os << "Pickup Station: " << event.pickup_st << "\n";
    os << "Dropoff Station: " << event.dropoff_st << "\n";
    os << "Pickup Geo: " << event.pickup << "\n";
    os << "Dropoff Geo: " << event.dropoff<< "\n";
    os << "Pickup Time: " << event.pickup_time << "\n";
    os << "Dropoff Time: " << event.dropoff_time;
    return os;
}

inline ostream &operator<<(ostream &os, vector<PickupEvent> const &events)
{
    for (int i = 0; i < events.size(); i++)
    {
        os << "\n[==================== PickupEvent (";
        os << i + 1 << "/" << events.size();
        os << ") ====================]" << "\n";
        os << events[i] << endl;
    }

    return os;
}

typedef KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<double, GeoPoints>,
    GeoPoints, 2> kd_tree_t;

void create_ts_files(kd_tree_t *index);
void create_prior(vector<ptime>& times, Prior& prior);
Prior **create_pairwise_priors(int n_stations);
Prior *create_priors(int n_stations);
GeoPoints load_stations();
vector<ptime> parse_ts_file(int p_st, int d_st);
vector<ptime> parse_region_ts_files(int st, int n_stations);
vector<PickupEvent> parse_historical_data(string fname, kd_tree_t *index,
        int rows);
size_t get_nearest(kd_tree_t *index, double lng, double lat);
kd_tree_t *create_kd_tree(GeoPoints pts);
kd_tree_t *create_stations_kd_tree();

#endif
