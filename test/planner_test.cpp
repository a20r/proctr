
#include <fstream>
#include "gtest/gtest.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/graph_reader.hpp"
#include "proctr/planner.hpp"
#include "proctr/rebalancer.hpp"

using namespace std;
using namespace boost::posix_time;

TEST(PlannerTest, InitialTest)
{
    int n_stations = 101;
    int max_cap = 10;
    double n_rates_rf = 100;
    double vol = 0;
    double resample_thresh = 0.1 * n_rates_rf;
    int rows = 100;

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
    vector<GeoPoint> idle, enroute;
    vector<int> enroute_free_seats;
    for (int i = 0; i < events.size() / 2; i++)
    {
        idle.push_back(events[i].pickup);
    }

    for (int i = events.size() / 2; i < events.size(); i++)
    {
        enroute.push_back(events[i].pickup);
        enroute_free_seats.push_back(7);
    }

    // updates the rates for 10 seconds with the new events and
    // and then rebalances the "idle" vehicles
    planner.update_rates(events, 10);
    RebalancingSolution sol = planner.rebalance(idle, enroute,
            enroute_free_seats);
    cout << sol << endl;
}

TEST(PlannerTest, RebalancerTest)
{
    RebalancerParams params;
    Rebalancer rebalancer(params);

    // using the pickup events as idle vehicle data as an example
    string nyc_data_fname = "data/nyc_taxi_data.csv";
    int rows = 100;
    vector<GeoPoint> regions = rebalancer.get_regions();
    vector<PickupEvent> events = parse_historical_data(
            nyc_data_fname, regions, rows);
    vector<GeoPoint> idle, enroute;
    vector<int> enroute_free_seats;
    for (int i = 0; i < events.size() / 2; i++)
    {
        idle.push_back(events[i].pickup);
    }

    for (int i = events.size() / 2; i < events.size(); i++)
    {
        enroute.push_back(events[i].pickup);
        enroute_free_seats.push_back(7);
    }

    rebalancer.update_rates(events, 10);
    RebalancingSolution sol = rebalancer.rebalance(idle, enroute,
            enroute_free_seats);
    cout << sol << endl;
}
