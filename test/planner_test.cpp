
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
    int rows = 10;
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

    WeightedGraph<GeoPoint, GeoPointHash> graph = read_graph(
            points_fname, edges_fname, durs_fname);

    vector<GeoPoint> regions = load_stations();
    vector<PickupEvent> events = parse_historical_data(fname, regions, rows);
    Planner planner(regions, max_cap, rfs, graph);
    vector<GeoPoint> locs;
    for (auto event : events)
    {
        locs.push_back(event.pickup);
    }
    planner.rebalance(locs);
}
