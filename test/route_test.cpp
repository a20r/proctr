
#include <ostream>
#include "gtest/gtest.h"
#include "proctr/route.hpp"

using namespace std;

TEST(RouteTest, SourceAndSink)
{
    Route r(1, 2);
    EXPECT_EQ(1, r.get_source());
    EXPECT_EQ(2, r.get_sink());
}
