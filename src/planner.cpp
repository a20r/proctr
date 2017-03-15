
#include <nanoflann.hpp>
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>
#include "proctr/graph.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

Planner::Planner()
{
}

Planner::Planner(vector<GeoPoint> regions, int cap, RateFilter *rate_filters,
        WeightedGraph<GeoPoint, GeoPointHash> graph) :
    regions(regions),
    n_stations(regions.size()),
    cap(cap),
    rate_filters(rate_filters),
    graph(graph),
    env(new GRBEnv)
{
    region_ids = get_all_nearest(graph.get_nodes(), regions);
}

Planner::~Planner()
{
    // delete env;
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

vector<size_t> Planner::get_all_nearest(vector<GeoPoint> refs,
        vector<GeoPoint> locs)
{
    vector<size_t> nearest(locs.size());

    for (int i = 0; i < locs.size(); i++)
    {
        nearest[i] = get_nearest(refs, locs[i]);
    }

    return nearest;
}

double Planner::graph_distance(GeoPoint src, GeoPoint sink)
{
    int src_node = get_nearest(graph.get_nodes(), src);
    int sink_node = get_nearest(graph.get_nodes(), sink);
    return graph.shortest_dist(src_node, sink_node);
}

MatrixXd Planner::get_costs(vector<GeoPoint> locs)
{
    MatrixXd costs = MatrixXd::Zero(locs.size(), n_stations);
    for (int i = 0; i < locs.size(); i++)
    {
        #pragma omp parallel for
        for (int j = 0; j < n_stations; j++)
        {
            costs(i, j) = graph_distance(locs[i],
                    graph.get_node(region_ids[j]));
        }
    }

    return costs;
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

double Planner::max_region_time_heuristic(int Nv, int Nr,
        MatrixXd costs, VectorXd rates, VectorXi enroute_seats)
{
    vector<int> n_vecs(Nr, 0);

    #pragma omp parallel for
    for (int v = 0; v < Nv; v++)
    {
        double min_cost = 0;
        int min_cost_region = 0;
        for (int r = 0; r < Nr; r++)
        {
            double cost = costs(v, r);
            if (r == 0 or cost < min_cost)
            {
                min_cost = cost;
                min_cost_region = r;
            }
        }

        #pragma omp critical
        n_vecs[min_cost_region]++;
    }

    double max_region_time = 0;
    #pragma omp parallel for
    for (int r = 0; r < Nr; r++)
    {
        double region_time = (enroute_seats[r] + cap * n_vecs[r]) / rates[r];
        if (region_time > max_region_time and rates[r] > 0)
        {
            max_region_time = region_time;
        }
    }

    return max_region_time;
}

VectorXi Planner::get_enroute_seats(vector<GeoPoint> enroute,
        vector<int> enroute_free_seats, int Nr)
{
    VectorXi enroute_seats = VectorXi::Zero(Nr);
    for (int i = 0; i < enroute.size(); i++)
    {
        int en_id = get_nearest(regions, enroute[i]);
        enroute_seats[en_id] = enroute_seats[en_id] + enroute_free_seats[i];
    }
    return enroute_seats;
}

RebalancingSolution Planner::rebalance(vector<GeoPoint> idle,
        vector<GeoPoint> enroute, vector<int> enroute_free_seats)
{
    int Nv = idle.size();
    int Nr = n_stations;
    cout << "gettings costs" << endl;
    MatrixXd costs = get_costs(idle);
    cout << "getting rates" << endl;
    VectorXd rates = get_rates(Nr);
    VectorXi caps = VectorXi::Constant(Nv, cap);
    cout << "enroute_seats" << endl;
    VectorXi enroute_seats = get_enroute_seats(enroute,
            enroute_free_seats, Nr);
    cout << "max_region_time" << endl;
    double max_region_time = max_region_time_heuristic(Nv, Nr, costs, rates,
            enroute_seats);
    vector<int> assignments;
    unordered_map<int, double> durs;
    cout << "creating model" << endl;
    RebalancingModel model = create_model(
            env, costs, rates, caps,
            enroute_seats,
            max_region_time, Nv, Nr);
    cout << "solving" << endl;
    model.solve(assignments, durs);
    cout << "done solving" << endl;
    vector<GeoPoint> assignment_locs(Nv);

    #pragma omp parallel for
    for (int i = 0; i < Nv; i++)
    {
        assignment_locs[i] = regions[assignments[i]];
    }
    RebalancingSolution sol = {
        costs, rates, caps, max_region_time,
        assignments, assignment_locs, durs, Nv, Nr};
    return sol;
}
