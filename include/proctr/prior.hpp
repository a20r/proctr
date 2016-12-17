
#ifndef PRIOR_HPP
#define PRIOR_HPP

#include <boost/date_time/posix_time/posix_time.hpp>
#include "proctr/kde.hpp"

using namespace boost::posix_time;

class Prior
{
    private:
        KDE kdes[7];

    public:
        Prior();
        ~Prior();
        int total_seconds(ptime time);
        double pdf(ptime time, double rate);
        double pdf(int day, time_duration time, double rate);
        void add_data(ptime time, double rate);
        vector<double> rates;
};

#endif
