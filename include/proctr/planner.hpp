#ifndef PROCTR_PLANNER_HPP
#define PROCTR_PLANNER_HPP

#include "proctr/prior.hpp"
#include "proctr/rate_filter.hpp"

class Planner
{
    public:
        Planner();
        Planner(int n_stations, RateFilter **rate_filters);
        ~Planner();

    private:
        int n_stations;
        RateFilter **rate_filters;
};

#endif
