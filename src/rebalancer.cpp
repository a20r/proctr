
#include "proctr/graph_reader.hpp"
#include "proctr/rebalancer.hpp"

Rebalancer::Rebalancer()
{
}

Rebalancer::Rebalancer(RebalancerParams params) : params(params)
{
    graph = read_graph(
            params.points_fname,
            params.edges_fname,
            params.durs_fname);
    priors = create_priors(params.n_regions);
    rfs = create_rate_filters(
            params.n_rates_rf,
            params.resample_thresh,
            params.n_regions,
            priors);
    regions = load_stations(params.regions_fname);
    planner = Planner(regions, params.max_cap, rfs, graph);
}

void Rebalancer::update_rates(vector<PickupEvent> events, int n_secs)
{
    planner.update_rates(events, n_secs);
}

RebalancingSolution Rebalancer::rebalance(vector<GeoPoint> idle,
        vector<GeoPoint> enroute, vector<int> enroute_free_seats)
{
    return planner.rebalance(idle, enroute, enroute_free_seats);
}

vector<GeoPoint> Rebalancer::get_regions()
{
    return regions;
}
