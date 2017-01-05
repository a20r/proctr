
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
    int n_stations = 101;
    int max_cap = 10;
    double n_rates_rf = 100;
    double vol = 0;
    double resample_thresh = 0.1 * n_rates_rf;
    int rows = 30;

    string fname = "data/nyc_taxi_data.csv";
    string points_fname = "data/nyc-graph/points.csv";
    string edges_fname = "data/nyc-graph/edges.csv";
    string durs_fname = "data/nyc-graph/week.csv";

    // creates graph
    WeightedGraph<GeoPoint, GeoPointHash> graph = read_graph(
            points_fname, edges_fname, durs_fname);

    // creates priors and rate filters and loads regions
    Prior *priors = create_priors(n_stations);
    RateFilter *rfs = create_rate_filters(n_rates_rf, resample_thresh,
            n_stations, priors);
    vector<GeoPoint> regions = load_stations();

    // pickup events used to rebalance
    vector<PickupEvent> events = parse_historical_data(fname, regions, rows);

    // planner instantiation
    Planner planner(regions, max_cap, rfs, graph);

    // using the pickup events as idle vehicle data as an example
    vector<GeoPoint> locs;
    for (auto event : events)
    {
        locs.push_back(event.pickup);
    }

    // updates the rates for 10 seconds with the new events and
    // and then rebalances the "idle" vehicles
    planner.update_rates(events, 10);
    RebalancingSolution sol = planner.rebalance(locs);
    cout << sol << endl;
}
