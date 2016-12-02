
#include "proctr/prior.hpp"

Prior::Prior()
{
}

Prior::~Prior()
{
    // delete[] kdes;
}

int Prior::total_seconds(tm time)
{
    return 60 * 60 * time.tm_hour + 60 * time.tm_min + time.tm_sec;
}

double Prior::pdf(tm time, double rate)
{
    double t_secs = total_seconds(time);
    return kdes[time.tm_wday].pdf(t_secs, rate);
}

void Prior::add_data(tm time, double rate)
{
    double t_secs = total_seconds(time);
    kdes[time.tm_wday].add_data(t_secs, rate);
}
