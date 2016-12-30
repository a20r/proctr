
#ifndef PRIOR_HPP
#define PRIOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include <limits>
#include "proctr/kde.hpp"

using namespace boost::posix_time;

class Prior
{
    private:
        KDE kdes[7];
        double min_rate = numeric_limits<double>::max();
        double max_rate = 0;

    public:
        Prior();
        ~Prior();
        int total_seconds(ptime time);
        double pdf(ptime time, double rate);
        double pdf(int day, time_duration time, double rate);
        void add_data(ptime time, double rate);
        double get_min_rate();
        double get_max_rate();
        vector<double> rates;
};

#endif
