
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
        int total_seconds(tm time);
        double pdf(tm time, double rate);
        void add_data(tm time, double rate);
};

#endif
