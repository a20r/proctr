
#include <ostream>
#include <cmath>
#include "gtest/gtest.h"
#include "cnpy.h"
#include "proctr/route.hpp"
#include "proctr/route_table.hpp"

using namespace std;

TEST(RouteTableTest, LoadingRouteTable)
{
    size_t num_ts = 101, counter = 0;
    double tol = 0.001;

    cnpy::NpyArray arr = cnpy::npy_load("data/mean_cors.npy");
    double *cors_data = reinterpret_cast<double *>(arr.data);
    RouteTable<double> cors(cors_data, 101);

    for (size_t p0 = 0; p0 < num_ts; p0++)
    {
        for (size_t d0 = 0; d0 < num_ts; d0++)
        {
            for (size_t p1 = 0; p1 < num_ts; p1++)
            {
                for (size_t d1 = 0; d1 < num_ts; d1++)
                {
                    Route r0(p0, d0);
                    Route r1(p1, d1);
                    double cor = cors.get(r0, r1);
                    double actual = cors_data[counter++];
                    ASSERT_TRUE(abs(cor - actual) < tol);
                }
            }
        }
    }
}

TEST(RouteTableTest, NotEqualTable)
{
    RouteTable<int> tab;
    bool loaded = RouteTable<int>::load_npy("data/test_not_equal.npy", tab);
    ASSERT_FALSE(loaded);
}

TEST(RouteTableTest, Not4DTable)
{
    RouteTable<int> tab;
    bool loaded = RouteTable<int>::load_npy("data/test_not_4d.npy", tab);
    ASSERT_FALSE(loaded);
}

TEST(RouteTableTest, LoadingNpy)
{
    RouteTable<double> tab;
    bool loaded = RouteTable<double>::load_npy("data/mean_cors.npy", tab);
    ASSERT_TRUE(loaded);
}
