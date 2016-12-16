
#include "gtest/gtest.h"
#include "proctr/data_reader.hpp"

using namespace std;

TEST(PriorTest, ReaderTest)
{
    GeoPoints gps = load_stations();
    cout << "Building tree" << endl;
    kd_tree_t index(2, gps, KDTreeSingleIndexAdaptorParams(1));
    index.buildIndex();
    cout << "Creating priors" << endl;
    create_ts_files(index);
}
