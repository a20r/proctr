
#include <fstream>
#include "gtest/gtest.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/graph_reader.hpp"
#include "proctr/planner.hpp"

using namespace std;
using namespace boost::posix_time;

TEST(PlannerTest, InitialTest)
{
    int n_stations = 10;
    int max_cap = 10;
    double max_rate = 0.01;
    double n_rates = 100;
    double vol = 0;
    double resample_thresh = 1;
    int rows = 1000;
    string fname = "data/nyc_taxi_data.csv";
    string points_fname = "data/nyc-graph/points.csv";
    string edges_fname = "data/nyc-graph/edges.csv";
    string durs_fname = "data/nyc-graph/week.csv";


    Prior *priors = create_priors(n_stations);
    RateFilter *rfs = new RateFilter[n_stations];

    for (int i = 0; i < n_stations; i++)
    {
        rfs[i] = RateFilter(max_rate, n_rates, vol, resample_thresh,
                priors[i]);
    }

    kd_tree_t *nodes_index;
    WeightedGraph<GeoPoint> graph = read_graph(
            points_fname, edges_fname, durs_fname, &nodes_index);

    GeoPoints regions = load_stations();
    kd_tree_t *regions_index = create_stations_kd_tree();
    vector<PickupEvent> events = parse_historical_data(fname, regions_index,
            rows);
    Planner planner(regions.pts, max_cap, rfs, regions_index, nodes_index,
            graph);
}
