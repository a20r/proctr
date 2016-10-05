
#include <ostream>
#include <cmath>
#include "gtest/gtest.h"
#include "proctr/route.hpp"
#include "proctr/route_table.hpp"
#include "proctr/predictor.hpp"

using namespace std;

TEST(PredictorTest, Creation)
{
    const size_t num_ts = 20;
    Correlations cors;
    Stds stds;
    Volumes vols;

    ASSERT_TRUE(Correlations::load_npy("test/data/mean_cors.npy", cors));
    ASSERT_TRUE(Stds::load_npy("test/data/stds.npy", stds));
    ASSERT_TRUE(Volumes::load_npy("test/data/volumes.npy", vols));

    Predictor<num_ts> pred(cors, vols, stds);
}
