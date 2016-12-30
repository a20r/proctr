
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
    int n_stations = 10;
    double max_rate = 0.01;
    double n_rates = 100;
    double vol = 0;
    double resample_thresh = 1;
    ptime starting_time(boost::gregorian::date(2014, 1, 1),
            time_duration(0, 0, 0));
    string fname = "data/nyc_taxi_data.csv";
    int rows = 1000;

    Prior *priors = create_priors(n_stations);
    RateFilter *rfs = new RateFilter[n_stations];

    for (int i = 0; i < n_stations; i++)
    {
        rfs[i] = RateFilter(max_rate, n_rates, vol, resample_thresh,
                priors[i], starting_time);
    }

    kd_tree_t *index = create_stations_kd_tree();
    vector<PickupEvent> events = parse_historical_data(fname, index, rows);
    cout << events << endl;
    Planner planner(n_stations, rfs, index);
}
