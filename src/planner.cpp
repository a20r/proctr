
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

Planner::Planner(int n_stations, RateFilter **rate_filters) :
    n_stations(n_stations),
    rate_filters(rate_filters)
{
}

Planner::~Planner()
{
}
