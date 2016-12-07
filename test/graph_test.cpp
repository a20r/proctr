
#include "gtest/gtest.h"
#include "proctr/graph.hpp"
#include "proctr/graph_reader.hpp"

using namespace std;

TEST(GraphTest, ReaderTest)
{
    string points_fname = "data/nyc-graph/points.csv";
    string edges_fname = "data/nyc-graph/edges.csv";
    string durs_fname = "data/nyc-graph/week.csv";
    WeightedGraph<LatLngNode> graph = read_graph(
            points_fname, edges_fname, durs_fname);
    ASSERT_TRUE(graph.get_nodes().size() == 4091);
    ASSERT_TRUE(graph.get_edges().size() == 9452);
    ASSERT_TRUE(graph.get_weights().size() == 9452);
}
