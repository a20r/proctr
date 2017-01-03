
#include <fstream>
#include "gtest/gtest.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/progress.hpp>
#include "proctr/prior.hpp"
#include "proctr/data_reader.hpp"

using namespace std;
using namespace boost::posix_time;

TEST(PriorTest, ReaderTest)
{
    vector<GeoPoint> regions = load_stations();
    create_ts_files(regions);
}

TEST(PriorTest, CreatePriorsTest)
{
    Prior *priors = create_priors(100);
}

TEST(PriorTest, DetermineRatesTest)
{
    Prior prior;
    vector<ptime> times = parse_region_ts_files(42, 100);
    create_prior(times, prior);

    time_duration dt = minutes(10);
    double min_rate = 0;
    double max_rate = prior.get_max_rate();
    double dr = (max_rate - min_rate) / 100;
    boost::progress_display show_progress(7 * 6 * 24);

    #pragma omp parallel for
    for (int i = 0; i < 7; i++)
    {
        ostringstream os;
        ofstream fout;
        os << "data/rates-" << i << ".csv";
        fout.open(os.str());
        time_duration cur_t = minutes(0);

        while (cur_t < hours(24))
        {
            double rate = min_rate;
            while (rate < max_rate)
            {
                fout << cur_t << "," << rate << "," <<
                    prior.pdf(i, cur_t, rate) << endl;
                rate += dr;
            }
            cur_t += dt;

            #pragma omp critical
            ++show_progress;
        }
        fout.close();
    }

}
