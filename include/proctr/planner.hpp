#ifndef PROCTR_PLANNER_HPP
#define PROCTR_PLANNER_HPP

#include <nanoflann.hpp>
#include <eigen3/Eigen/Dense>
#include "gurobi_c++.h"
#include "proctr/graph.hpp"
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/rate_filter.hpp"
#include "proctr/passive_rebalancing.hpp"

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
        vector<int> rebalance(vector<GeoPoint> locs);

    private:
        vector<GeoPoint> regions;
        vector<size_t> region_ids;
        int n_stations;
        int cap;
        RateFilter *rate_filters;
        WeightedGraph<GeoPoint, GeoPointHash> graph;
        GRBEnv *env;
};

#endif
