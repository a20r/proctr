
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

int Route::get_source()
{
    return source;
}

int Route::get_sink()
{
    return sink;
}
