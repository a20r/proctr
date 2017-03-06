#ifndef PROCTR_PLANNER_HPP
#define PROCTR_PLANNER_HPP

#include <nanoflann.hpp>
#include <eigen3/Eigen/Dense>
#include <unordered_map>
#include "gurobi_c++.h"
#include "proctr/graph.hpp"
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/rate_filter.hpp"
#include "proctr/passive_rebalancing.hpp"

struct RebalancingSolution
{
    MatrixXd costs;
    VectorXd rates;
    VectorXi caps;
    double max_region_time;
    vector<int> assignments;
    vector<GeoPoint> assignment_locs;
    unordered_map<int, double> durations;
    int Nv, Nr;
};

struct VehicleState
{
    GeoPoint location;
    int free_seats;
};

class Planner
{
    public:
        Planner();
        Planner(vector<GeoPoint> regions, int cap, RateFilter *rate_filters,
            WeightedGraph<GeoPoint, GeoPointHash> graph);
        ~Planner();
        void update_rates(vector<PickupEvent> &events, int secs);
        double graph_distance(GeoPoint src, GeoPoint sink);
        MatrixXd get_costs(vector<GeoPoint> locs);
        vector<size_t> get_all_nearest(vector<GeoPoint> refs,
            vector<GeoPoint> locs);
        VectorXd get_rates(int Nr);
        double max_region_time_heuristic(int Nv, int Nr, MatrixXd costs,
                VectorXd rates, VectorXi enroute_seats);
        VectorXi get_enroute_seats(vector<GeoPoint> enroute,
                vector<int> enroute_free_seats, int Nr);
        RebalancingSolution rebalance(vector<GeoPoint> idle,
                vector<GeoPoint> enroute, vector<int> enroute_free_seats);


    private:
        vector<GeoPoint> regions;
        vector<size_t> region_ids;
        int n_stations;
        int cap;
        RateFilter *rate_filters;
        WeightedGraph<GeoPoint, GeoPointHash> graph;
        GRBEnv *env;
};

inline ostream &operator<<(ostream &os, RebalancingSolution const &sol)
{
    os << "Region time upper bound: " << sol.max_region_time << endl;
    os << "Assignments:" << endl;
    for (int v = 0; v < sol.Nv; v++)
    {
        int assign = sol.assignments[v];
        os << "\t" << v << " --> " << assign;
        os << " " << sol.assignment_locs[v];
        os << " for " << sol.durations.at(assign);
        os << " secs" << endl;
    }
    return os;
}

#endif
