
#include <nanoflann.hpp>
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

Planner::Planner(int n_stations, RateFilter *rate_filters, kd_tree_t *index) :
    n_stations(n_stations),
    rate_filters(rate_filters),
    index(index)
{
}

Planner::~Planner()
{
}

void Planner::update_rates(vector<PickupEvent> &events)
{
}
