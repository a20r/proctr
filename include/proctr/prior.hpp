
#ifndef PRIOR_HPP
#define PRIOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <limits>
#include "proctr/kde.hpp"

using namespace boost::posix_time;

class Prior
{
    private:
        ptime last_time;
        double last_rate = -1;
        double d_sum = 0;
        double d_sum_sq = 0;
        int n_data = 0;
        double min_rate = numeric_limits<double>::max();
        double max_rate = 0;
        KDE kdes[7];

    public:
        Prior();
        ~Prior();
        int total_seconds(ptime time);
        double pdf(ptime time, double rate);
        double pdf(int day, time_duration time, double rate);
        void add_data(ptime time, double rate);
        double get_min_rate();
        double get_max_rate();
        double get_volatility();
        vector<double> rates;
};

#endif
