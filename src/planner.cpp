
#include <nanoflann.hpp>
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include "proctr/graph.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

Planner::Planner(vector<GeoPoint> regions, int cap, RateFilter *rate_filters,
        WeightedGraph<GeoPoint, GeoPointHash> graph) :
    regions(regions),
    n_stations(regions.size()),
    cap(cap),
    rate_filters(rate_filters),
    graph(graph),
    env(new GRBEnv)
{
}

Planner::~Planner()
{
    delete env;
}

void Planner::update_rates(vector<PickupEvent> &events, int secs)
{
    unordered_map<int, int> n_obs;
    ptime min_time, max_time;
    for (int i = 0; i < events.size(); i++)
    {
        if (!n_obs.count(events[i].pickup_st))
        {
            n_obs[events[i].pickup_st] = 0;
        }

        ptime pickup_time = events[i].pickup_time;

        if (i == 0)
        {
            min_time = pickup_time;
        }

        if (pickup_time < min_time)
        {
            min_time = pickup_time;
        }

        n_obs[events[i].pickup_st]++;
    }

    for (auto n_obs_pair : n_obs)
    {
        int st = n_obs_pair.first;
        int count = n_obs_pair.second;
        rate_filters[st].observe(count, min_time, secs);
    }
}

vector<size_t> Planner::get_all_nearest(kd_tree_t *index,
        vector<GeoPoint> locs)
{
    vector<size_t> nearest;
    for (auto loc : locs)
    {
        size_t near = get_nearest(index, loc.lng, loc.lat);
        nearest.push_back(near);
    }
    return nearest;
}

double Planner::graph_distance(GeoPoint src, GeoPoint sink)
{
    int src_node = get_nearest(graph.get_nodes(), src);
    int sink_node = get_nearest(graph.get_nodes(), sink);
    cout << src_node << ", " << sink_node << endl;
    return graph.shortest_dist(src_node, sink_node);
}

MatrixXd Planner::get_costs(vector<GeoPoint> locs)
{
    // Write this shit mate!
}

VectorXd Planner::get_rates(int Nr)
{
    VectorXd rates = VectorXd::Zero(Nr);

    for (int i = 0; i < Nr; i++)
    {
        rates[i] = rate_filters[i].get_predicted_rate();
    }

    return rates;
}

vector<int> Planner::rebalance(vector<GeoPoint> locs)
{
    int Nv = locs.size();
    int Nr = n_stations;
    VectorXd rates = get_rates(Nr);
    VectorXd caps = VectorXd::Constant(cap, Nv);
}
