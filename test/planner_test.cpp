
#include <fstream>
#include "gtest/gtest.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"
#include "proctr/planner.hpp"

using namespace std;
using namespace boost::posix_time;

TEST(PlannerTest, InitialTest)
{
    int n_stations = 100;
    double max_rate = 0.01;
    double n_rates = 100;
    double vol = 0;
    double resample_thresh = 1;
    ptime starting_time(boost::gregorian::date(2013, 1, 1),
            time_duration(0, 0, 0));
    Prior **priors = create_priors(n_stations);
    RateFilter **rfs = new RateFilter *[n_stations];
    for (int i = 0; i < n_stations; i++)
    {
        rfs[i] = new RateFilter[n_stations];
        for (int j = 0; j < n_stations; j++)
        {
            rfs[i][j] = RateFilter(max_rate, n_rates, vol, resample_thresh,
                    &priors[i][j], starting_time);
        }
    }

    vector<PickupEvent> parse_historical_data("data/nyc_taxi_data.csv", 100);

    Planner planner(n_stations, rfs);
}
