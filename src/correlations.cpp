
#include <cmath>
#include "proctr/route.hpp"
#include "proctr/correlations.hpp"

Correlations::Correlations()
{
}

Correlations::Correlations(double *cors, int num_ts) :
    cors(cors), num_ts(num_ts)
{
}

Correlations::~Correlations()
{
}

size_t Correlations::get_index(Route r0, Route r1)
{
    return r0.get_source() * pow(num_ts, 3)
        + r0.get_sink() * pow(num_ts, 2)
        + r1.get_source() * num_ts + r1.get_sink();
}

double Correlations::get_correlation(Route r0, Route r1)
{
    size_t ind = get_index(r0, r1);
    return cors[ind];
}
