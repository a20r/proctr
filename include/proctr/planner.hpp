#ifndef PROCTR_PLANNER_HPP
#define PROCTR_PLANNER_HPP

#include <nanoflann.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/rate_filter.hpp"

class Planner
{
    public:
        Planner();
        Planner(int n_stations, RateFilter *rate_filters, kd_tree_t *index);
        ~Planner();
        void update_rates(vector<PickupEvent> &events);

    private:
        kd_tree_t *index;
        int n_stations;
        RateFilter *rate_filters;
};

#endif
