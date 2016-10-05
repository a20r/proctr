
#include <ostream>
#include <cmath>
#include "gtest/gtest.h"
#include "cnpy.h"
#include "proctr/route.hpp"
#include "proctr/route_table.hpp"

using namespace std;

TEST(RouteTableTest, LoadingRouteTable2D)
{
    size_t num_ts = 20, counter = 0;
    double tol = 0.001;
    cnpy::NpyArray arr = cnpy::npy_load("test/data/mean_cors.npy");
    double *cors_data = reinterpret_cast<double *>(arr.data);
    RouteTable<double> cors(cors_data, num_ts);

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

TEST(RouteTableTest, LoadingRouteTable1D)
{
    size_t num_ts = 20, counter = 0;
    cnpy::NpyArray arr = cnpy::npy_load("test/data/volume.npy");
    int *vols_data = reinterpret_cast<int *>(arr.data);
    RouteTable<int, 1> vols(vols_data, num_ts);

    for (size_t p0 = 0; p0 < num_ts; p0++)
    {
        for (size_t d0 = 0; d0 < num_ts; d0++)
        {
            Route r0(p0, d0);
            int vol = vols.get(r0);
            int actual = vols_data[counter++];
            ASSERT_TRUE(vol == actual);
        }
    }
}

TEST(RouteTableTest, NotEqualTable)
{
    RouteTable<int> tab;
    bool loaded = RouteTable<int>::load_npy("test/data/test_not_equal.npy", tab);
    ASSERT_FALSE(loaded);
}

TEST(RouteTableTest, Not2DTable)
{
    RouteTable<int> tab;
    bool loaded = RouteTable<int>::load_npy("test/data/test_not_2d.npy", tab);
    ASSERT_FALSE(loaded);
}

TEST(RouteTableTest, Not1DTable)
{
    RouteTable<int, 1> tab;
    bool loaded = RouteTable<int, 1>::load_npy("test/data/test_not_equal.npy", tab);
    ASSERT_FALSE(loaded);
}

TEST(RouteTableTest, LoadingNpy)
{
    RouteTable<double> tab;
    bool loaded = RouteTable<double>::load_npy("test/data/mean_cors.npy", tab);
    ASSERT_TRUE(loaded);
}

TEST(RouteTableTest, Loading2DNpy)
{
    RouteTable<int, 1> tab;
    bool loaded = RouteTable<int, 1>::load_npy("test/data/volume.npy", tab);
    ASSERT_TRUE(loaded);
}

TEST(RouteTableTest, LoadingVolumesWithAlias)
{
    Volumes vols;
    bool loaded = Volumes::load_npy("test/data/volume.npy", vols);
    ASSERT_TRUE(loaded);
}

TEST(RouteTableTest, LoadingStdsWithAlias)
{
    Stds stds;
    bool loaded = Volumes::load_npy("test/data/stds.npy", stds);
    ASSERT_TRUE(loaded);
}

TEST(RouteTableTest, LoadingCorsWithAlias)
{
    Correlations cors;
    bool loaded = Correlations::load_npy("test/data/mean_cors.npy", cors);
    ASSERT_TRUE(loaded);
}
