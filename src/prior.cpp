
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
    tm t = to_tm(time);
    double t_secs = total_seconds(time);
    return kdes[t.tm_wday].pdf(t_secs, rate);
}

double Prior::pdf(int day, time_duration time, double rate)
{
    return kdes[day].pdf(time.total_seconds(), rate);
}

void Prior::add_data(ptime time, double rate)
{
    if (rate < min_rate)
    {
        min_rate = rate;
    }

    if (rate > max_rate)
    {
        max_rate = rate;
    }

    tm t = to_tm(time);
    double t_secs = total_seconds(time);
    kdes[t.tm_wday].add_data(t_secs, rate);
    rates.push_back(rate);
}

double Prior::get_min_rate()
{
    return min_rate;
}

double Prior::get_max_rate()
{
    return max_rate;
}
