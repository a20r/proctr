
#include "gtest/gtest.h"
#include "proctr/route.hpp"

TEST(RouteTest, SourceAndSink)
{
    Route<int> r(1, 2);
    EXPECT_EQ(1, r.get_source());
    EXPECT_EQ(2, r.get_sink());
}
