#ifndef PROCTR_REBALANCER_HPP
#define PROCTR_REBALANCER_HPP

#include "proctr/graph.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

struct RebalancerParams
{
    int n_regions = 101;
    int max_cap = 10;
    int n_rates_rf = 100;
    double resample_thresh = 0.1 * n_rates_rf;

    string points_fname = "data/nyc-graph/points.csv";
    string edges_fname = "data/nyc-graph/edges.csv";
    string durs_fname = "data/nyc-graph/week.csv";
    string regions_fname = "data/stations.csv";

    void set_n_regions(int n_regions)
    {
        this->n_regions = n_regions;
    }

    void set_max_cap(int max_cap)
    {
        this->max_cap = max_cap;
    }

    void set_n_rates_rf(int n_rates_rf)
    {
        this->n_rates_rf = n_rates_rf;
    }

    void set_resample_thresh(double resample_thresh)
    {
        this->resample_thresh = resample_thresh;
    }

    void set_points_fname(string fname)
    {
        this->points_fname = fname;
    }

    void set_edges_fname(string fname)
    {
        this->edges_fname = fname;
    }

    void set_durs_fname(string fname)
    {
        this->durs_fname = fname;
    }

    void set_regions_fname(string fname)
    {
        this->regions_fname = fname;
    }
};

class Rebalancer
{
    public:
        RebalancerParams params;
        Rebalancer();
        Rebalancer(RebalancerParams params);
        void update_rates(vector<PickupEvent> events, int n_secs);
        RebalancingSolution rebalance(vector<GeoPoint> idle,
                vector<GeoPoint> enroute, vector<int> enroute_free_seats);
        vector<GeoPoint> get_regions();

    private:
        WeightedGraph<GeoPoint, GeoPointHash> graph;
        Prior *priors;
        RateFilter *rfs;
        vector<GeoPoint> regions;
        Planner planner;
};

#endif
