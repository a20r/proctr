
#ifndef ROUTE_HASH_HPP
#define ROUTE_HASH

#include "proctr/route.hpp"
#include "cnpy.h"

using namespace std;

template<size_t dim>
class RouteHash
{
    public:
        size_t operator() (const Route &r) const
        {
            const int source = r.get_source();
            const int sink = r.get_sink();
            return hash<size_t>()(dim * source + sink);
        }
};

#endif
