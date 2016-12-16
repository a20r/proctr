#ifndef PROCTR_DATA_READER_HPP
#define PROCTR_DATA_READER_HPP

#include <nanoflann.hpp>

using namespace nanoflann;

const double earth_radius = 6371;

class GeoPoint
{
    public:
        double  lng, lat;
        GeoPoint(double lng, double lat) : lng(lng), lat(lat) {}
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
        bool kdtree_get_bbox(BBOX& /* bb */) const { return false; }
};

typedef KDTreeSingleIndexAdaptor<
    L2_Simple_Adaptor<double, GeoPoints>,
    GeoPoints, 2> kd_tree_t;

void create_ts_files(kd_tree_t &index);
GeoPoints load_stations();

#endif
