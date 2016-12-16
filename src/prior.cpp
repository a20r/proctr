
#include "proctr/prior.hpp"

Prior::Prior()
{
}

Prior::~Prior()
{
    // delete[] kdes;
}

int Prior::total_seconds(ptime time)
{
    tm t = to_tm(time);
    return 60 * 60 * t.tm_hour + 60 * t.tm_min + t.tm_sec;
}

double Prior::pdf(ptime time, double rate)
{
    double t_secs = total_seconds(time);
    return kdes[time.tm_wday].pdf(t_secs, rate);
}

void Prior::add_data(ptime time, double rate)
{
    double t_secs = total_seconds(time);
    kdes[time.tm_wday].add_data(t_secs, rate);
}
