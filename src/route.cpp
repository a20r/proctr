
#include "proctr/route.hpp"

Route::Route()
{
}

Route::~Route()
{
}

Route::Route(int source, int sink) : source(source), sink(sink)
{
}

int Route::get_source() const
{
    return source;
}

int Route::get_sink() const
{
    return sink;
}

bool Route::operator<(const Route &rhs)
{
    if (source >= rhs.get_source())
    {
        return false;
    }

    if (source < rhs.get_source())
    {
        return true;
    }

    if (source == rhs.get_source() and sink < rhs.get_sink())
    {
        return true;
    }

    return false;
}

void Route::next(int num_ts)
{
    if (sink + 1 >= num_ts)
    {
        source++;
        sink = 0;
    }
    else
    {
        sink++;
    }
}
