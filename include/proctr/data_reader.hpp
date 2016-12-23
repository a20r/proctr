#ifndef PROCTR_DATA_READER_HPP
#define PROCTR_DATA_READER_HPP

#include <nanoflann.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "proctr/prior.hpp"

using namespace nanoflann;

const double earth_radius = 6371;

class GeoPoint
{
    public:
        double  lng, lat;

        GeoPoint(double lng, double lat) : lng(lng), lat(lat)
        {
        }

        GeoPoint()
        {
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

class PickupEvent
{
    public:
        GeoPoint pickup, dropoff;
        ptime pickup_time, dropoff_time;

        PickupEvent()
        {
        }

        PickupEvent(GeoPoint pickup, GeoPoint dropoff,
                ptime pickup_time, ptime dropoff_time) :
            pickup(pickup), dropoff(dropoff),
            pickup_time(pickup_time), dropoff_time(dropoff_time)
        {
        }

        bool operator<(const PickupEvent& other) const
        {
            return pickup_time < other.pickup_time;
        }
};

typedef KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<double, GeoPoints>,
    GeoPoints, 2> kd_tree_t;

void create_ts_files(kd_tree_t &index);
void create_prior(vector<ptime>& times, Prior& prior);
Prior **create_priors(int n_stations);
GeoPoints load_stations();
vector<ptime> parse_ts_file(int p_st, int d_st);
vector<PickupEvent> parse_historical_data(string fname, int rows)

#endif
