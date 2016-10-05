
#ifndef ROUTE_TABLE_HPP
#define ROUTE_TABLE_HPP

#include <cmath>
#include <initializer_list>
#include <sys/stat.h>
#include "cnpy.h"
#include "proctr/route.hpp"

using namespace std;

template<typename T, size_t dim = 2>
class RouteTable
{
    public:
        RouteTable()
        {
        }

        RouteTable(T *data, int num_ts) :
            data(data), num_ts(num_ts)
        {
        }

        ~RouteTable()
        {
        }

        size_t get_num_ts()
        {
            return num_ts;
        }

        inline size_t get_index(Route routes[dim])
        {
            size_t index = 0;
            for (size_t i = 0; i < dim; i++)
            {
                index += routes[i].get_source() *
                    pow(num_ts, 2 * (dim - i) - 1);
                index += routes[i].get_sink() *
                    pow(num_ts, 2 * (dim - i) - 2);
            }
            return index;
        }

        T get(Route r0, Route r1)
        {
            if (dim == 2)
            {
                Route routes[] = {r0, r1};
                size_t ind = get_index(routes);
                return data[ind];
            }
            throw std::runtime_error("Cannot call this getter with dim = 2");
        }

        T get(Route r0)
        {
            if (dim == 1)
            {
                Route routes[] = {r0};
                size_t ind = get_index(routes);
                return data[ind];
            }
            throw std::runtime_error("Cannot call this getter with dim = 1");
        }

        template<typename D, size_t d = 2>
        static bool load_npy(string fname, RouteTable<D, d> &table)
        {
            struct stat buffer;
            if (stat(fname.c_str(), &buffer)) {
                return false;
            }

            cnpy::NpyArray arr = cnpy::npy_load(fname);

            if (arr.shape.size() != 2 * d)
            {
                return false;
            }

            for (size_t i = 0; i < 2 * d - 1; i++)
            {
                if (arr.shape[i] != arr.shape[i + 1])
                {
                    return false;
                }
            }

            D *data = reinterpret_cast<D *>(arr.data);
            table = RouteTable<D, d>(data, arr.shape[0]);
            return true;
        }

    private:
        T *data;
        int num_ts;
};

using Volumes = RouteTable<int, 1>;
using Stds = RouteTable<double, 2>;
using Correlations = RouteTable<double, 2>;

#endif
